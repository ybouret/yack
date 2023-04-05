/opt/local/bin cmake build-gcc    && \
ninja -C build-gcc                && \
/opt/local/bin/cmake build-icc    && \
ninja -C build-icc
