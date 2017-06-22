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

function fn_get_program_id()
{

    local type_id_file=$1
    local program_result_file_all="${TMP_CONTENT_FILE}_program_all"
    local program_result_file="${TMP_CONTENT_FILE}_program"
    local program_series_number_file="${TMP_CONTENT_FILE}_program_series_number"
    local program_id_file="${TMP_CONTENT_FILE}_program_id"
    local program_name
    local program_value 
    local program_count
    local pg_cmd="select a.create_time, a.name, a.series_number, a.id, c.type_id from t_program a, t_series_program b, t_program_type_ref_series c where c.series_id = b.series_id and b.program_id = a.id and a.name != '';"

    :>${program_id_file}

    echo "${pg_cmd}" > "${pg_sql_file}"
    fn_exe_sql_pg "${pg_dbname}" "${pg_sql_file}" "${program_result_file_all}" || return $?

    cat ${program_result_file_all}  |egrep -v "^[[:space:]]*id|^[[:space:]]*---|.*row.*\)$|^$" > ${TMP_CONTENT_FILE}
    mv ${TMP_CONTENT_FILE} ${program_result_file_all}

    while read type_id
    do
        [ -z "${type_id}" ] && continue

        cat ${program_result_file_all}  |  awk -v id=${type_id} -F"|"  '{if ($5 == id) print $1"|"$2"|"$3"|"$4}' > ${TMP_CONTENT_FILE}

        mv ${TMP_CONTENT_FILE} ${program_result_file}

        cat ${program_result_file} | awk -F"|" '{print $2"|"$3}' | while read program_value
        do
            program_count=$(grep "${program_value}" "${program_result_file}" | wc -l)

            echo "${type_id}|${program_value}"

            if [ ${program_count}  -gt 1 ]
            then
                cat ${program_result_file} |grep "${program_value}"  | sort  -nr | awk -F"|" '{print $4}' | sed '1d'  >> ${program_id_file}_${type_id}
            fi
        done
        break
    done < ${type_id_file}

    cat ${program_id_file}* | uniq > ${TMP_CONTENT_FILE}
    mv ${TMP_CONTENT_FILE} ${program_id_file}

    eval $2="${program_id_file}"

    return 0
}


function fn_del_program_id()
{
    local program_id_file=$1
    local program_id_failed_file=${TMP_CONTENT_FILE}_program_id_failed
    local all_progam_id

    :>${program_id_failed_file}
    clear

    while read id
    do
       python del_program_id.py "${id}" &> ${TMP_CONTENT_FILE}

       if ! grep "\"returnCode\": \"0\"" ${TMP_CONTENT_FILE}  &>/dev/null
       then
           echo "Failed to del id:${id}" >> ${program_id_failed_file}
       else
           echo $"Success to del id:${id}"
       fi

    done <${program_id_file}

    echo "======================================================================================================"
    [ -s "${program_id_failed_file}" ] && {  echo -e "\nFailed to del id:\n"; cat ${program_id_failed_file} ; }
    cp ${TMP_CONTENT_FILE}  result_file
    cp ${program_id_failed_file} failed_file

    return 0
}


function fn_main()
{

    local ref_program_id_file
    local pg_dbname="dcmp"

    local pg_sql_file="${TMP_CONTENT_FILE}_pg_sql"

    fn_check_pg || return $?

    local type_id__result_file="${TMP_CONTENT_FILE}_type_id"
    local pg_cmd="SELECT distinct(type_id) from t_program_type_ref_series;"
    echo "${pg_cmd}" > ${pg_sql_file}

    fn_exe_sql_pg "${pg_dbname}" "${pg_sql_file}" "${type_id__result_file}" || return $?

    cat ${type_id__result_file}  |egrep -v "^[[:space:]]*type_id|^[[:space:]]*---|.*row.*\)$|^$" | sed 's#||#| |#g' > ${TMP_CONTENT_FILE}
    mv ${TMP_CONTENT_FILE} ${type_id__result_file}

    fn_get_program_id "${type_id__result_file}" ref_program_id_file || return $?

    fn_del_program_id "${ref_program_id_file}" 

    return 0
}

fn_main $@
RET=$?
rm -rf ${TMP_CONTENT_FILE}*

exit ${RET}



