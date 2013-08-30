VER=`cat VERSION`
TANO_EXPORT_BRANDING="-DBRANDING=${CUSTOM}"
PWD=`pwd`
BUILD_DIR="${PWD}/release/build/tano"
OUTPUT_DIR="${PWD}/release/output/tano"

build() 
{
    say "Building Tano"

    if [ "${CUSTOM}" != "" ]; then
        echo "Branded build: ${CUSTOM}"

        VER=`cat src/branding/${CUSTOM}/VERSION`
        BUILD_DIR="${PWD}/release/build/${CUSTOM}"
        OUTPUT_DIR="${PWD}/release/output/${CUSTOM}"

        export TANO_EXPORT_BRANDING
    fi

    if [ -d "${OUTPUT_DIR}" ]; then
        rm -r ${OUTPUT_DIR}
    fi
    mkdir -p ${BUILD_DIR}
    mkdir -p ${OUTPUT_DIR}

    pushd ${BUILD_DIR}
    cmake ../../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${OUTPUT_DIR} -DLIBVLCQT_INCLUDE_DIR=${VLCQT}/include -DLIBVLCQT_LIBRARY=${VLCQT}/lib/libvlc-qt.dylib -DLIBVLCQT_WIDGETS_LIBRARY=${VLCQT}/lib/libvlc-qt-widgets.dylib ${TANO_EXPORT_BRANDING} -DGIT=
    make install -j8
    popd

    say "Deploying Tano"

    pushd ${OUTPUT_DIR}
    sh deploy.sh
    cp *.dmg ../../
    popd

    say "Tano deployment completed!"
}

eval build
