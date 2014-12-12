/*
 * Mesa 3-D graphics library
 * Version:  6.5.3
 *
 * Copyright (C) 1999-2007  Brian Paul   All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * \file  programopt.c 
 * Vertex/Fragment program optimizations and transformations for program
 * options, etc.
 *
 * \author Brian Paul
 */


#include "main/glheader.h"
#include "main/imports.h"
#include "main/mtypes.h"
#include "prog_parameter.h"
#include "prog_statevars.h"
#include "program.h"
#include "programopt.h"
#include "prog_instruction.h"

/**
 * Scan/rewrite program to remove reads of custom (output) registers.
 * The passed type has to be either PROGRAM_OUTPUT or PROGRAM_VARYING
 * (for vertex shaders).
 * In GLSL shaders, varying vars can be read and written.
 * On some hardware, trying to read an output register causes trouble.
 * So, rewrite the program to use a temporary register in this case.
 */
void
_mesa_remove_output_reads(struct gl_program *prog, gl_register_file type)
{
   GLuint i;
   GLint outputMap[VERT_RESULT_MAX];
   GLuint numVaryingReads = 0;

   assert(type == PROGRAM_VARYING || type == PROGRAM_OUTPUT);
   assert(prog->Target == GL_VERTEX_PROGRAM_ARB || type != PROGRAM_VARYING);

   for (i = 0; i < VERT_RESULT_MAX; i++)
      outputMap[i] = -1;

   /* look for instructions which read from varying vars */
   for (i = 0; i < prog->NumInstructions; i++) {
      struct prog_instruction *inst = prog->Instructions + i;
      const GLuint numSrc = _mesa_num_inst_src_regs(inst->Opcode);
      GLuint j;
      for (j = 0; j < numSrc; j++) {
         if (inst->SrcReg[j].File == type) {
            /* replace the read with a temp reg */
            const GLuint var = inst->SrcReg[j].Index;
            if (outputMap[var] == -1) {
               numVaryingReads++;
               outputMap[var] = _mesa_find_free_register(prog,
                                                         PROGRAM_TEMPORARY);
            }
            inst->SrcReg[j].File = PROGRAM_TEMPORARY;
            inst->SrcReg[j].Index = outputMap[var];
         }
      }
   }

   if (numVaryingReads == 0)
      return; /* nothing to be done */

   /* look for instructions which write to the varying vars identified above */
   for (i = 0; i < prog->NumInstructions; i++) {
      struct prog_instruction *inst = prog->Instructions + i;
      const GLuint numSrc = _mesa_num_inst_src_regs(inst->Opcode);
      GLuint j;
      for (j = 0; j < numSrc; j++) {
         if (inst->DstReg.File == type &&
             outputMap[inst->DstReg.Index] >= 0) {
            /* change inst to write to the temp reg, instead of the varying */
            inst->DstReg.File = PROGRAM_TEMPORARY;
            inst->DstReg.Index = outputMap[inst->DstReg.Index];
         }
      }
   }

   /* insert new instructions to copy the temp vars to the varying vars */
   {
      struct prog_instruction *inst;
      GLint endPos, var;

      /* Look for END instruction and insert the new varying writes */
      endPos = -1;
      for (i = 0; i < prog->NumInstructions; i++) {
         struct prog_instruction *inst = prog->Instructions + i;
         if (inst->Opcode == OPCODE_END) {
            endPos = i;
            _mesa_insert_instructions(prog, i, numVaryingReads);
            break;
         }
      }

      assert(endPos >= 0);

      /* insert new MOV instructions here */
      inst = prog->Instructions + endPos;
      for (var = 0; var < VERT_RESULT_MAX; var++) {
         if (outputMap[var] >= 0) {
            /* MOV VAR[var], TEMP[tmp]; */
            inst->Opcode = OPCODE_MOV;
            inst->DstReg.File = type;
            inst->DstReg.Index = var;
            inst->SrcReg[0].File = PROGRAM_TEMPORARY;
            inst->SrcReg[0].Index = outputMap[var];
            inst++;
         }
      }
   }
}
