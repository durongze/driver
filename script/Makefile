sh_script=mysh
sh_script_exec=$(sh_script).exe
sh_script_srcs=$(sh_script).cc

sh2cc_exec = sh2cc
sh2cc_srcs = $(wildcard sh2cc.cc)
sh2cc_objs = $(patsubst %.cc,out/$(platform)/%.o,$(sh2cc_srcs))

env_exec = env
env_srcs = $(wildcard env.cc)
env_objs = $(patsubst %.cc,out/$(platform)/%.o,$(env_srcs))

platform=linux

CXX=g++
CC =gcc
RM =rm -rf
MKDIR =mkdir -p

CXX_FLAGS +=-DENV_MAIN

default:$(sh_script_exec) $(sh2cc_exec) $(env_exec)

$(sh_script_srcs):$(sh2cc_exec) $(sh_script)
	./$(sh2cc_exec) $(sh_script)

$(env_exec): $(env_objs) $(sh_script_srcs)
	$(CXX) $(CXX_FLAGS) $(env_objs) $(sh_script_srcs) -o $(env_exec) 

$(sh2cc_exec):$(sh2cc_objs) 
	$(CXX) $(sh2cc_objs) -o $(sh2cc_exec)

$(sh_script_exec):$(sh_script_srcs)
	$(CXX) $(sh_script_srcs) -o $(sh_script_exec) 

clean:
	$(RM) $(sh2cc_objs) $(env_objs) 
	$(RM) $(sh2cc_exec) $(env_exec)
	$(RM) $(sh_script_srcs)
	$(RM) $(sh_script_exec)
	
out/$(platform)/%.o:%.cc
	$(MKDIR)  $(dir $@)
	$(CXX) -c $< -o $@
