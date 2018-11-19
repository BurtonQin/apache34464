#!/usr/bin/env bash
# instrument apache34464

HOME=/home/tzt77/Develop/ComAir/cmake-build-debug

WORK=$(pwd)/build/

BC_FILE=${WORK}/apache34464.0.0.preopt.bc

MERGE_BC=${WORK}/test.merge.bc

PRE_BC_FILE=${WORK}/test.pre.bc

BC_ID_FILE=${WORK}/test.id.bc

BC_MARK_FILE=${WORK}/test.mark.bc

BC_PROF_FILE=${WORK}/test.mark.aprof.bc

PRE_INLINE_BC_FILE=${WORK}/test.pre.inline.bc

INLINE_BC_FILE=${WORK}/test.inline.bc

O_PROF_FILE=${WORK}/test.aprof.o

NO_OPT_FILE=${WORK}/apache34464.no.opt

FUNC_NAME_ID=${WORK}/apache34464_func_name_id.txt

{

  echo "merge return......"
  /HDD/llvm5.0/install/bin/opt -mergereturn ${BC_FILE} > ${MERGE_BC}

  echo "tag-id....."
 /HDD/llvm5.0/install/bin/opt -load ${HOME}/lib/IDAssigner/libIDAssignerPass.so -tag-id ${MERGE_BC} > ${BC_ID_FILE}

 echo "mark-flags....."
 /HDD/llvm5.0/install/bin/opt -load ${HOME}/lib/MarkFlagForAprof/libMarkFlagForAprofPass.so -mark-flags -not-optimize 1 ${BC_ID_FILE} > ${BC_MARK_FILE}

 echo "instrument-hooks....."
 /HDD/llvm5.0/install/bin/opt -load ${HOME}/lib/AprofHook/libAProfHookPass.so -instrument-hooks -not-optimize 1 -strFileName ${FUNC_NAME_ID} ${BC_MARK_FILE} > ${BC_PROF_FILE}

   echo "link-runtime-lib....."
  /HDD/llvm5.0/install/bin/llvm-link ${BC_PROF_FILE} /home/tzt77/Develop/ComAir/runtime/libs/AProfHooks/prof-hook.o -o \
  ${PRE_INLINE_BC_FILE}

 echo "build-to-executable....."
 libtool --preserve-dup-deps --tag=CXX --mode=link clang++  -flto -use-gold-plugin -Wl,-plugin-opt=save-temps -Xclang -disable-O0-optnone -g -O0 -DSAFE_MUTEX  \
 -fno-implicit-templates -fno-exceptions -fno-rtti  -rdynamic  -o ${NO_OPT_FILE}  ${PRE_INLINE_BC_FILE} \
-lz -ldl -lpthread -lcrypt -lnsl -lm  -lpthread -lrt

 echo "you can execute it......"

}
