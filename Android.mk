# don't include LOCAL_PATH for submodules

include $(CLEAR_VARS)
LOCAL_MODULE    := loax
LOCAL_CFLAGS    := -Wall -D$(A3D_CLIENT_VERSION)
LOCAL_SRC_FILES := loax/loax_server.c loax/loax_server_cmd.c loax/loax_server_gl2.c loax/loax_log.c loax/loax_serialize.c
LOCAL_LDLIBS    := -Llibs/armeabi -llog
LOCAL_SHARED_LIBRARIES := liba3d libnet

include $(BUILD_SHARED_LIBRARY)
