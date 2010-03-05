#!/bin/bash

function pushd()
{
    builtin pushd "${@}" &>/dev/null
}

function popd()
{
    builtin popd &>/dev/null
}

function GetAndCompile_QtPropertyBrowser()
{
    QT_PROP_BRWSR_PKG="qtpropertybrowser-2.5_1-opensource"
    QT_PROP_BRWSR_URL="http://get.qt.nokia.com/qt/solutions/lgpl/${QT_PROP_BRWSR_PKG}.tar.gz"

    [[ -d ${QT_PROP_BRWSR_PKG} ]] || wget -O - "${QT_PROP_BRWSR_URL}" |tar xz

    pushd "${QT_PROP_BRWSR_PKG}" && trap 'popd' RETURN

    ./configure -library && qmake && make
}

function Get_mid9th_toolbox()
{
    BASE_URL="http://github.com/martyone/mid9th/raw/master"

    function update()
    {
        URL=${1?}

        OFILE=$(basename ${URL})
        wget ${URL} -O ${OFILE} && chmod a+x ${OFILE}
    }

    cd "$(dirname "${0}")"

    update ${BASE_URL}/prj/AutoQMake.sh
    update ${BASE_URL}/prj/template.sh
}

cd "$(dirname "${0}")"

shopt -s extglob
shopt -s nullglob

for waste in !($(basename "${0}"))
do
    read -p "About to remove <${waste}> ...OK? [y/N] " YN
    [[ ${YN} == y ]] && rm -rf "${waste}"
done

for action in \
    GetAndCompile_QtPropertyBrowser \
    Get_mid9th_toolbox
do
    read -p "About to execute action <${action}> ...OK? [Y/n]" YN
    [[ ${YN} != n ]] && ${action}
done
