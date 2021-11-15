#!/bin/bash

function AutoInstall()
{
    SrcDir=$1
    DstDir=$2
    CurFlags="$3"
    InstallDir=$(echo $SrcDir | tr -s "." "_")
    pushd $SrcDir
    if [ -f CMakeLists.txt ];then
        mkdir dyzbuild  
        pushd dyzbuild
        echo -e "\033[32m export CXXFLAGS=\"-fPIC\" && cmake .. -DCMAKE_INSTALL_PREFIX=$DstDir/$InstallDir $CurFlags \033[0m"
        export CXXFLAGS="-fPIC" && cmake .. -DCMAKE_INSTALL_PREFIX=$DstDir/$InstallDir $CurFlags 
        make  || { echo "$FUNCNAME $LINENO failed,${FUNCNAME[1]} ${BASH_LINENO[1]} "; exit 1; }
        make install  || { echo "$FUNCNAME $LINENO failed,${FUNCNAME[1]} ${BASH_LINENO[1]} "; exit 1; }
        popd
    elif [ -f setup.py ];then
        sudo python setup.py install 
    elif [ -f autogen.sh ];then
        ./autogen.sh
        make distclean
        mkdir dyzbuild
        pushd dyzbuild
        dos2unix ../configure && export CXXFLAGS="-fPIC"
        echo -e "\033[32m ../configure --prefix=$DstDir/$InstallDir $CurFlags \033[0m" 
        ../configure --prefix=$DstDir/$InstallDir $CurFlags  || { echo "$FUNCNAME $LINENO failed,${FUNCNAME[1]} ${BASH_LINENO[1]} "; exit 1; }
        make   || { echo "$FUNCNAME $LINENO failed,${FUNCNAME[1]} ${BASH_LINENO[1]} "; exit 1; }
        make install   || { echo "$FUNCNAME $LINENO failed,${FUNCNAME[1]} ${BASH_LINENO[1]} "; exit 1; }
        popd
    elif [ -f configure ];then
        mkdir dyzbuild
        pushd dyzbuild
        dos2unix ../configure && export CXXFLAGS="-fPIC"
        echo -e "\033[32m ../configure --prefix=$DstDir/$InstallDir $CurFlags \033[0m" 
        ../configure --prefix=$DstDir/$InstallDir $CurFlags  || { echo "$FUNCNAME $LINENO failed,${FUNCNAME[1]} ${BASH_LINENO[1]} "; exit 1; }
        make   || { echo "$FUNCNAME $LINENO failed,${FUNCNAME[1]} ${BASH_LINENO[1]} "; exit 1; }
        make install   || { echo "$FUNCNAME $LINENO failed,${FUNCNAME[1]} ${BASH_LINENO[1]} "; exit 1; }
        popd
    elif [ -f Makefile ];then
        make $CurFlags || { echo "$FUNCNAME $LINENO failed,${FUNCNAME[1]} ${BASH_LINENO[1]} "; exit 1; }
    elif [ -f meson.build ];then
	meson setup build
	meson build --prefix=$DstDir/$InstallDir $CurFlags || { echo "$FUNCNAME $LINENO failed,${FUNCNAME[1]} ${BASH_LINENO[1]} "; exit 1;  }  
	pushd build
	ninja -j8
	popd
    else
        echo -e "\033[31m Install $InstallDir Fail, cfg file not exist !!! \033[0m"
        exit 1
    fi
    popd
}

function TarXFFile()
{
    SrcDir=$1
    DstDir=$2
    CurFlags="$3"
    pushd ./
    SrcFileList=$SrcDir
    for file in $SrcFileList
    do
        case $file in
            *.tar.*)
                FileDir=$(tar -tf $file | cut -f1 -d'/' | uniq | sed -n '1p')
                echo -e "\033[32mFile:\033[0m$file \033[32mDir:\033[0m$FileDir"
                tar xf $file
                AutoInstall "$FileDir" "$DstDir" "$CurFlags"
                ;;
            *.zip)
                FileDir=$(unzip -v $file |awk '{print $8}' | grep "/$" | uniq | sed -n '1p' | awk -F'/' '{print $1}')
                FileName=${file%.*}
                echo -e "\033[32mFile:$file Dir:$FileDir FileName:$FileName\033[0m"
                if [ "$FileDir" = "$FileName" ];then
                    unzip -q $file 
                else
                    mkdir $FileName
                    unzip -q $file -d $FileName
                    DstDir=$FileName
                fi
                AutoInstall "$FileDir" "$DstDir" "$CurFlags"
                ;;
        esac;
    done
    popd
}

####################################环境变量配置#####################################

function GenFileNameByFile()
{
    file="$1"
    case $file in
        *.tar.*)
            FileDir=$(tar -tf $file | cut -f1 -d'/' | uniq | sed -n '1p')
            ;;
        *.zip)
            #FileDir=$(unzip -v $file |awk '{print $8}' | grep "/" | uniq | sed -n '1p' | awk -F'/' '{print $1}')
            FileDir=$(unzip -v $file |awk '{print $8}' | grep "/$" | uniq | sed -n '1p' | awk -F'/' '{print $1}')
            ;;
    esac;
    FileDir=$(echo $FileDir | tr -s "." "_")
    echo $FileDir
}

function GenFileNameVar()
{
    FileList="$(ls *.zip )"
    FileDir=""

    echo "" >env.txt
    for file in $FileList
    do
        FileDir=$(GenFileNameByFile $file)
        echo $FileDir >>env.txt
    done
}

function GenEnvVar()
{
    BASHRC="${HOME}/.bashrc"
    echo "fileList=\"$(cat env.txt)\""  >>${BASHRC}
    echo "for tmpFile in \${fileList}" >>${BASHRC}
    echo "do" >>${BASHRC}
    echo "    TMP_FILE_HOME=\${HOME}/opt/\${tmpFile}" >>${BASHRC}
    echo "    export C_INCLUDE_PATH=\${TMP_FILE_HOME}/include:\${C_INCLUDE_PATH}" >>${BASHRC}
    echo "    export CPLUS_INCLUDE_PATH=\${TMP_FILE_HOME}/include:\${CPLUS_INCLUDE_PATH}" >>${BASHRC}
    echo "    export LIBRARY_PATH=\${TMP_FILE_HOME}/lib:\${LIBRARY_PATH}" >>${BASHRC}
    echo "    export LIBRARY_PATH=\${TMP_FILE_HOME}/lib64:\${LIBRARY_PATH}" >>${BASHRC}
    echo "    export LD_LIBRARY_PATH=\${TMP_FILE_HOME}/lib:\${LD_LIBRARY_PATH}" >>${BASHRC}
    echo "    export LD_LIBRARY_PATH=\${TMP_FILE_HOME}/lib64:\${LD_LIBRARY_PATH}" >>${BASHRC}
    echo "    export PKG_CONFIG_PATH=\${TMP_FILE_HOME}/lib/pkgconfig/:\${PKG_CONFIG_PATH}" >>${BASHRC}
    echo "    export PKG_CONFIG_PATH=\${TMP_FILE_HOME}/lib64/pkgconfig/:\${PKG_CONFIG_PATH}" >>${BASHRC}
    echo "    export CMAKE_MODULE_PATH=\${TMP_FILE_HOME}/lib/cmake/:\${CMAKE_MODULE_PATH}" >>${BASHRC}
    echo "    export PATH=\${TMP_FILE_HOME}/bin:\${TMP_FILE_HOME}/sbin:\$PATH" >>${BASHRC}
    echo "done" >>${BASHRC}
}

#GenFileNameVar 
#GenEnvVar 

#TarXFFile "icu-release.zip" "${HOME}/opt/" ""
#TarXFFile "tiff-4.3.0.zip" "${HOME}/opt/" "" 
#TarXFFile "leptonica.zip" "${HOME}/opt/" "" 
#TarXFFile "pcre2-10.39.tar.gz" "${HOME}/opt/" "" 
#TarXFFile "pcre-8.45.tar.gz" "${HOME}/opt/" " -DBUILD_SHARED_LIBS=true " 
#TarXFFile "libffi-3.4.2.tar.gz" "${HOME}/opt/" "" 
#TarXFFile "zlib-1.2.11.tar.gz" "${HOME}/opt/" "" 
#TarXFFile "freetype-2.11.0.tar.gz" "${HOME}/opt/" " -DBUILD_SHARED_LIBS=true " 
#TarXFFile "harfbuzz-3.1.1.tar.gz" "${HOME}/opt/" "" 
#TarXFFile "fontconfig-2.13.94.tar.xz" "${HOME}/opt/" "" 
#TarXFFile "libpng-1.6.37.tar.gz" "${HOME}/opt/" "" 
#TarXFFile "libjpeg-turbo-2.1.1.tar.gz" "${HOME}/opt/" "" 
#TarXFFile "pixman-0.40.0.tar.gz" "${HOME}/opt/" "" 
#TarXFFile "cairo-1.17.4.tar.xz" "${HOME}/opt/" "" 
TarXFFile "pango-1.49.3.tar.gz" "${HOME}/opt/" "" 
#TarXFFile "tesseract.zip" "${HOME}/opt/" ""
