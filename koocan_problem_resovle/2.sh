#!/bin/bash

ABSOLUTE_PATH=$(dirname $0)
cd ${ABSOLUTE_PATH}
TMP_CONTENT_FILE="${ABSOLUTE_PATH}/tmp_sql_file_`date +%s`_$$"


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

    fn_check_pg || reteurn $?

    su - ${pg_user} -c "psql -d ${pg_name} -c \"${pg_sql}\"" 1> ${pg_cmd_file}

    return 0

}


function fn_main()
{
    local tmp_dbname="cms"
    local tmp_cmd=" SELECT c.title,c.program_type, a.note, a.code, a.storage_name, a.created_timestamp  from streaming a, program_streaming_map b, program c where a.code = b.streaming_code and b.program_code = c.code and  b.result_code != 0 and a.vod_tvod_flag = 0 order by a.created_timestamp desc;"

    fn_exe_sql_pg "${tmp_dbname}" "${tmp_cmd}" "${TMP_CONTENT_FILE}" || return $?

    [ -f "${TMP_CONTENT_FILE}" ] && cat ${TMP_CONTENT_FILE}

    return 0
}

fn_main $@
RET=$?
[ -f "${TMP_CONTENT_FILE}" ] && rm -rf ${TMP_CONTENT_FILE}

exit ${RET}



