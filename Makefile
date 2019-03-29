OBJS_DIR = objs_tmp
CPPFLAGS = -std=c++11
CPPFLAGS += -MMD -MP
LDFLAGS = -lthrift 

# add -I and -L if headers or libs are not installed in system path
# CPPFLAGS += -I"your include path"
# LDFLAGS += -L"your lib path"

VPATH=./ ./gen-cpp

SRC_COMM = $(notdir $(wildcard gen-cpp/*.cpp))
SRC_NOTESTORE_SERVER_SKELETON = $(SRC_COMM) NoteStore_server.skeleton.cpp
SRC_USERSTORE_SERVER_SKELETON = $(SRC_COMM) UserStore_server.skeleton.cpp
SRC_EVERNOTECLIENT = $(SRC_COMM) EvernoteClient.cpp

OBJS_NOTESTORE_SERVER_SKELETON = $(SRC_NOTESTORE_SERVER_SKELETON:%.cpp=$(OBJS_DIR)/%.o)
OBJS_USERSTORE_SERVER_SKELETON = $(SRC_USERSTORE_SERVER_SKELETON:%.cpp=$(OBJS_DIR)/%.o)
OBJS_EVERNOTECLIENT = $(SRC_EVERNOTECLIENT:%.cpp=$(OBJS_DIR)/%.o)

DEPS_NOTESTORE_SERVER_SKELETON = $(SRC_NOTESTORE_SERVER_SKELETON:%.cpp=$(OBJS_DIR)/%.d)
DEPS_USERSTORE_SERVER_SKELETON = $(SRC_USERSTORE_SERVER_SKELETON:%.cpp=$(OBJS_DIR)/%.d)
DEPS_EVERNOTECLIENT = $(SRC_EVERNOTECLIENT:%.cpp=$(OBJS_DIR)/%.d)

all: directories skeleton client

skeleton: userstore_skeleton notestore_skeleton

directories: $(OBJS_DIR)
$(OBJS_DIR):
	@mkdir -p $@

userstore_skeleton: $(OBJS_USERSTORE_SERVER_SKELETON)
	g++ $(CPPFLAGS) $^ -o $@ $(LDFLAGS)

notestore_skeleton: $(OBJS_NOTESTORE_SERVER_SKELETON)
	g++ $(CPPFLAGS) $^ -o $@ $(LDFLAGS)

client: $(OBJS_EVERNOTECLIENT)
	g++ $(CPPFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJS_DIR)/%.o: %.cpp
	g++ -c $(CPPFLAGS) $< -o $@

$(OBJS_DIR)/%.d: ;

# show how to compile .thrift file into cpp source file.
# thrift 0.12.0 is required, currently it's the newest version.
gencpp:
	@versionok=$$(thrift --version | grep 0.12.0); \
	if [ -z "$$versionok" ]; then \
		echo "Thrift version mismatched, v0.12.0 is required!!!"; \
		exit; \
	fi
	thrift -r --gen cpp:no_skeleton ./IDL/NoteStore.thrift
	thrift -r --gen cpp:no_skeleton ./IDL/UserStore.thrift

clean:
	rm userstore_skeleton notestore_skeleton client \
	   $(OBJS_DIR)/NoteStore_server.skeleton.* $(OBJS_DIR)/UserStore_server.skeleton.* \
	   $(OBJS_DIR)/EvernoteClient.* -fr
cleandata:
	rm evernote-data/* -fr
distclean: clean cleandata
	rm $(OBJS_DIR) -fr

-include $(DEPS_NOTESTORE_SERVER_SKELETON)
-include $(DEPS_USERSTORE_SERVER_SKELETON)	
-include $(DEPS_EVERNOTECLIENT)