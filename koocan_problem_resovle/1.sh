#!/bin/bash

ABSOLUTE_PATH=$(dirname $0)
cd ${ABSOLUTE_PATH}
ABSOLUTE_PATH=$(pwd)
TMP_CONTENT_FILE="${ABSOLUTE_PATH}/tmp_sql_file_`date +%s`_$$"
cd - 1>/dev/null


function fn_check_pg()
{
    local pg_user="postgres"

    su - ${pg_user} -c "psql -c \"\l\"" 1>/dev/null

    if [ $? -ne 0 ]
    then
        ehco "Faied to login the pgsql!"
        return 1
    fi

    return 0

}

function fn_check_redis()
{
    local tmp_redis_ip=$1
    local tmp_redis_port=$2

    if [ -z "${tmp_redis_ip}" -o -z "${tmp_redis_port}" ]
    then
        echo "The parameter redis server ip<${tmp_redis_ip}> or redis port<${tmp_redis_port}> is null!"
        return 1
    fi

    if ! which redis-cli &>/dev/null
    then
        echo "No redis client founed!"
        return 1
    fi

    if [ -n "${redis_pvalue}" ]
    then
        redis-cli -h ${tmp_redis_ip} -p ${tmp_redis_port}  -a ${redis_pvalue} <<EOF  > ${TMP_CONTENT_FILE}
info
EOF
    else
        redis-cli -h ${tmp_redis_ip} -p ${tmp_redis_port}  <<EOF  > ${TMP_CONTENT_FILE}
info
EOF
    fi
    if [ ! -s ${TMP_CONTENT_FILE} ]
    then
        echo "Failed to connect redis server!"
        return 1
    fi

    return 0
}

function fn_exe_sql_redis()
{
    local tmp_redis_ip=$1
    local tmp_redis_port=$2
    local tmp_redis_cmd=$3
    local tmp_redis_file=$4

    if [ -z "${tmp_redis_ip}" -o -z "${tmp_redis_port}" -o -z "${tmp_redis_cmd}" ]
    then
        echo "The parameter redis server ip<${tmp_redis_ip}> or redis port<${tmp_redis_port}>  or redis cmd<${tmp_redis_cmd}> is null!"
        return 1
    fi

    if [ -n "${redis_pvalue}" ]
    then
        redis-cli -h ${tmp_redis_ip} -p ${tmp_redis_port}  -a ${redis_pvalue}  <<EOF  > ${tmp_redis_file}
${tmp_redis_cmd}
EOF
    else
        redis-cli -h ${tmp_redis_ip} -p ${tmp_redis_port}  <<EOF  > ${tmp_redis_file}
${tmp_redis_cmd}
EOF
    fi

    return 0

}

function fn_exe_sql_pg()
{
    local pg_user="postgres"
    local pg_name=$1
    local pg_sql=$2
    local pg_cmd_file=$3

    if [ -z "${pg_name}" ] || [ -z "${pg_sql}" ] || [ -z "${pg_cmd_file}" ]
    then
        echo "The paramter pg_name<${pg_name}> or pg_sql<${pg_sql}> or pg_cmd_file<${pg_cmd_file}> is null!"
        return 1
    fi

    if ! id ${pg_user} &>/dev/null
    then
        echo "Failed to get pg user!"
        return 1
    fi

    su - ${pg_user} -c "psql -d ${pg_name} -f ${pg_sql}" 1> ${pg_cmd_file}

    return 0

}

function fn_compare_data()
{
    local pg_data_file=$1
    local redis_data_file=$2
    local redis_media_code

    while read redis_media_code
    do
        grep -w ${redis_media_code} ${pg_data_file}

        if [ $? -ne 0 ]
        then
            continue
        fi

        sed -i "/${redis_media_code}/d" ${pg_data_file}

    done < ${redis_data_file}

    return 0;
}

function fn_valid_ip()
{
    local ip=$1

    echo ${ip} |egrep "([0-9]{1,3}[\.]){3}[0-9]{1,3}"  &>/dev/null

    if [ $? -ne 0 ]
    then
        echo "The ip<${ip}> is invalid!"
        return 1
    fi

    return 0
}

function fn_check_ip()
{
    local ip=$1

    ping -c3 ${ip} &>/dev/null

    if [ $? -ne 0 ]
    then
        echo "The ip<${ip}> is unreachable!"
        return 1
    fi

    return 0

}

function fn_to_json()
{
    local ori_file=$1
    local json_file=${TMP_CONTENT_FILE}_json
    local i j tmp

    key_list=("code"  "release_flag"  "streaming_bit_rate"  "abstract"  "duration"  "vod_tvod_flag"  "created_timestamp"  "id"  "updated_timestamp"  "storage_file_location"  "title"  "sequence"  "note"  "filmtype"  "filesize"  "file_source_URI"  "encode_type"  "description"  "cs_url"  "publish_flag"  "HD_SD_flag"  "streaming_coded"  "language"  "storage_name"  "checksum"  "short_title"  "program_code")

    : > ${TMP_CONTENT_FILE}
    : > ${json_file}

    cat ${ori_file} | while read line
    do
        
        i=0
        while [ ${i} -lt 27 ]
        do
            let j=i+1

            tmp=$(echo "${line}" | awk -v num=${j} -F"|" '{print $num }')
            eval ${key_list[$i]}="${tmp}"  2>/dev/null
            echo \"${key_list[$i]}\":"\"${tmp}\"" 2>/denull >> ${TMP_CONTENT_FILE}
            let i++
        done
        cat ${TMP_CONTENT_FILE} | sed 's#"null"#null#' |tr '\n'  ',' |sed 's#^#{#g' | sed  's#,$#}\n#g'  1>> ${json_file}

        :>${TMP_CONTENT_FILE}

    done

    eval $2=${json_file}
    return 0

}

function fn_send_file()
{
    local send_file=$1
    local send_info
    local media_code
    local line=1
    local server_info="http://198.255.68.18/api/dcmp/streaming?code="
    local tmp_exec_file=${TMP_CONTENT_FILE}_exec
    local tmp_send_result_file=${TMP_CONTENT_FILE}_send

    cat ${media_code_file}
    while read send_info
    do

        media_code=$(cat ${media_code_file} | sed 's# ##g' | sed -n "${line}p")

        echo -e "\n================================================================================================================"
        echo "curl -d '${send_info}' -X PUT ${server_info}${media_code}" | sed 's#"null"#null#g' > ${tmp_exec_file}
        cat "${tmp_exec_file}"
        echo -e "================================================================================================================"
        . ${tmp_exec_file} | tee ${TMP_CONTENT_FILE}

        if ! egrep  "{\"errorcode\": 1}" ${TMP_CONTENT_FILE} &>/dev/null
        then
            echo "echo -e \"\033[31m Failed  to send:${media_code}\033[0m\"\"\"" >> ${tmp_send_result_file}
        else
            echo "echo -e \"\033[36m Succeed to send:${media_code}\033[0m\"\"\"" >> ${tmp_send_result_file}
        fi
        let line++

    done < "${send_file}"
     
    clear
    echo -e "\n-------------------result:----------------------"
    . ${tmp_send_result_file}

    return 0
}

function fn_main()
{
    local timestamp=$1
    local redis_server_ip=$2
    local redis_port=$3
    local redis_pvalue=$4
    local redis_cmd
    local REF_PG_FILE
    local redis_result_file="${TMP_CONTENT_FILE}_redis"
    local all_media_code_result_file="${TMP_CONTENT_FILE}_all_media_code"

    :> ${all_media_code_result_file}

    if [ -z "${redis_server_ip}" -o -z "${redis_port}" ]
    then
        echo -e "\tUsage: bash $0 redis_server_ip redis_port\n"
        return 1
    fi

    [ -z "${timestamp}" ] && timestamp=7
    fn_valid_ip "${redis_server_ip}" || return $?  fn_check_ip "${redis_server_ip}" || return $?

    local pg_dbname="cms"

    local pg_cmd="SELECT a.code, b.mediacode, b.program_code from  (select * from (SELECT  (current_timestamp::date - updated_timestamp::date ) count, code, updated_timestamp from streaming) as streaming_new where count <= ${timestamp}) a, program_streaming_map b where a.code = b.streaming_code and a.code != '' and b.mediacode != 'undefined' order by count;"

    local pg_result_file="${TMP_CONTENT_FILE}_pg"
    local media_code_file="${TMP_CONTENT_FILE}_media_code"

    local pg_sql_file="${TMP_CONTENT_FILE}_pg_sql"
    echo "${pg_cmd}" > ${pg_sql_file}

    fn_check_pg || return $?

    fn_check_redis "${redis_server_ip}" "${redis_port}"|| return $?

    fn_exe_sql_pg "${pg_dbname}" "${pg_sql_file}" "${pg_result_file}" || return $?

    cat ${pg_result_file}  |egrep -v "^[[:space:]]*code|^[[:space:]]*-----|.*row.*\)$|^$" | sed 's#|[[:space:]]*#|#g' | sed 's#[[:space:]]*|#|#g' | sed 's#||#|null|#g' > ${TMP_CONTENT_FILE}
    cat ${TMP_CONTENT_FILE} |egrep -v "^\|\||^\|null\|" > ${pg_result_file}

    cat ${pg_result_file} | awk -F"|" '{print $3" "$2}' | while read pg_program_code pg_media_code
    do
        redis_cmd="HGET program:${pg_program_code} streaming"
        fn_exe_sql_redis "${redis_server_ip}" "${redis_port}" "${redis_cmd}" "${redis_result_file}"|| return $?
        cat "${redis_result_file}" | tr ',' '\n'  | grep -w "${pg_media_code}" 1>/dev/null
        if [ $? -ne 0 ]
        then
            echo ${pg_media_code} >> ${all_media_code_result_file}
        fi

    done 

    if [ -s "${all_media_code_result_file}" ]
    then
        all_media_code=$(cat "${all_media_code_result_file}"| grep -v "^$"  | sed -e "s#^#'#g ; s#\$#'#g; s#\r\n# #g" | sed ':label;N;s/\n/,/;b label')

        local pg_cmd="SELECT c.code, c.release_flag, c.streaming_bit_rate, c.abstract, c.duration, c.vod_tvod_flag, c.created_timestamp, c.id, c.updated_timestamp, c.storage_file_location, c.title, b.sequence, c.note, c.filmtype, c.filesize, c.\"file_source_URI\", c.encode_type, c.description, c.cs_url, c.publish_flag, c.\"HD_SD_flag\", c.streaming_coded, c.language, c.storage_name, c.checksum, c.short_title, b.program_code from streaming c, program_streaming_map b where c.code = b.streaming_code and b.mediacode in (${all_media_code});"

        echo "${pg_cmd}" > ${pg_sql_file}

        fn_exe_sql_pg "${pg_dbname}" "${pg_sql_file}" "${pg_result_file}" || return $?

        cat ${pg_result_file}  |egrep -v "^[[:space:]]*code|^[[:space:]]*-----|.*row.*\)$|^$" | sed 's#|[[:space:]]*#|#g' | sed 's#[[:space:]]*|#|#g' | sed 's#||#|null|#g' > ${TMP_CONTENT_FILE}
        cat ${TMP_CONTENT_FILE} |egrep -v "^\|\||^\|null\|" > ${pg_result_file}

        cat ${pg_result_file} |awk -F"|" '{print $1}' > ${media_code_file}

        if [ -s "${pg_result_file}" ]
        then
            fn_to_json ${pg_result_file} REF_PG_FILE || return $?

            fn_send_file "${REF_PG_FILE}" || return $?
        else
            echo -e "\nFailed to get data from postgres"
            return 1
        fi
     
    else
        echo "====================================================="
        echo -e "\nEvery is OK,no need to repair!"
    fi

    return 0
}

fn_main $@
RET=$?
rm -rf ${TMP_CONTENT_FILE}*

exit ${RET}



