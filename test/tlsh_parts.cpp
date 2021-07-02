/*
 * TLSH is provided for use under two licenses: Apache OR BSD.
 * Users may opt to use either license depending on the license
 * restictions of the systems with which they plan to integrate
 * the TLSH code.
 */ 

/* ==============
 * Apache License
 * ==============
 * Copyright 2013 Trend Micro Incorporated
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* ===========
 * BSD License
 * ===========
 * Copyright (c) 2013, Trend Micro Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.

 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

//////////////////////////////////////////////////////////////////////////
//
// (C) Trend Micro
// written Jon Oliver 2021
//
//////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <errno.h>

#include "tlsh.h"
#include "tlsh_impl.h"

////////////////////////////////////////////////////////////////////////////////

static void usage()
{
	printf("usage: tlsh_parts -d <digest>\n" );
	exit(0);
}

static void show_digest(char *digest, int start, int nchar)
{
	printf("%s\n", digest);
	for (int x=0; x<start+nchar; x++) {
		if (x < start)
			printf(" ");
		else 
			printf("^");
	}
	printf("\n");
}

int main(int argc, char *argv[])
{
	char *digest		= NULL;
	int argIdx		= 1;
	while (argc > argIdx) {
		if (strcmp(argv[argIdx], "-d") == 0) {
			digest = argv[argIdx+1];
			argIdx = argIdx+2;
		} else {
			printf("\nunknown option '%s'\n\n", argv[argIdx]);
			usage();
		}
	}
	if (digest == NULL)
		usage();

	Tlsh d;
	int err = d.fromTlshStr(digest);
	if (err) {
		printf("invalid TLSH: %s\n", digest);
		exit(1);
	}
	show_digest(digest, 2, 2);
	printf("Checksum(0)		%d\n", d.Checksum(0) );
	printf("----\n");
	show_digest(digest, 4, 2);
	printf("Lvalue()		%d\n", d.Lvalue() );
	printf("----\n");
	show_digest(digest, 6, 1);
	printf("Q1ratio()		%d\n", d.Q1ratio() );
	printf("----\n");
	show_digest(digest, 7, 1);
	printf("Q2ratio()		%d\n", d.Q2ratio() );
	printf("----\n");
	for (int bi=0; bi<EFF_BUCKETS; bi=bi+2) {
		show_digest(digest, 8+bi/2, 1);
		printf("BucketValue(%3d)	%d\n", bi,   d.BucketValue(bi) );
		printf("BucketValue(%3d)	%d\n", bi+1, d.BucketValue(bi+1) );
		printf("----\n");
	}
}
