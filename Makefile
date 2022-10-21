# Copyright 2022 gab
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License

SOURCES = $(shell find ast global lexer logger parser -name '*.cc')
HEADERS = $(shell find ast global lexer logger parser -name '*.h')
OBJ = ${SOURCES:.cpp=.o}

CC = clang++
CFLAGS = -g -O3 -I llvm/include -I llvm/build/include -I ./
LLVMFLAGS = `/usr/bin/llvm-config --cxxflags --ldflags --system-libs --libs all`

.PHONY: main

main: driver.cc ${OBJ}
	${CC} ${CFLAGS} ${LLVMFLAGS} ${OBJ} $< -o toy

clean:
	rm -r ${OBJ}

%.o: %.cc ${HEADERS}
	${CC} ${CFLAGS} ${LLVMFLAGS} -c $< -o $@