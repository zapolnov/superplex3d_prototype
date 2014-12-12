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
 * \file program.c
 * Vertex and fragment program support functions.
 * \author Brian Paul
 */


#include "main/glheader.h"
#include "main/imports.h"
#include "main/mtypes.h"
#include "main/hash.h"
#include "program.h"
#include "prog_parameter.h"
#include "prog_instruction.h"


/**
 * A pointer to this dummy program is put into the hash table when
 * glGenPrograms is called.
 */
struct gl_program _mesa_DummyProgram;


/**
 * Set the vertex/fragment program error state (position and error string).
 * This is generally called from within the parsers.
 */
void
_mesa_set_program_error(GLcontext *ctx, GLint pos, const char *string)
{
   ctx->Program.ErrorPos = pos;
   _mesa_free((void *) ctx->Program.ErrorString);
   if (!string)
      string = "";
   ctx->Program.ErrorString = _mesa_strdup(string);
}


/**
 * Find the line number and column for 'pos' within 'string'.
 * Return a copy of the line which contains 'pos'.  Free the line with
 * _mesa_free().
 * \param string  the program string
 * \param pos     the position within the string
 * \param line    returns the line number corresponding to 'pos'.
 * \param col     returns the column number corresponding to 'pos'.
 * \return copy of the line containing 'pos'.
 */
const GLubyte *
_mesa_find_line_column(const GLubyte *string, const GLubyte *pos,
                       GLint *line, GLint *col)
{
   const GLubyte *lineStart = string;
   const GLubyte *p = string;
   GLubyte *s;
   int len;

   *line = 1;

   while (p != pos) {
      if (*p == (GLubyte) '\n') {
         (*line)++;
         lineStart = p + 1;
      }
      p++;
   }

   *col = (pos - lineStart) + 1;

   /* return copy of this line */
   while (*p != 0 && *p != '\n')
      p++;
   len = p - lineStart;
   s = (GLubyte *) _mesa_malloc(len + 1);
   _mesa_memcpy(s, lineStart, len);
   s[len] = 0;

   return s;
}


/**
 * Initialize a new vertex/fragment program object.
 */
static struct gl_program *
_mesa_init_program_struct( GLcontext *ctx, struct gl_program *prog,
                           GLenum target, GLuint id)
{
   (void) ctx;
   if (prog) {
//      GLuint i;
      _mesa_bzero(prog, sizeof(*prog));
//      prog->Id = id;
      prog->Target = target;
//      prog->Resident = GL_TRUE;
      prog->RefCount = 1;
//      prog->Format = GL_PROGRAM_FORMAT_ASCII_ARB;

      /* default mapping from samplers to texture units */
//      for (i = 0; i < MAX_SAMPLERS; i++)
//         prog->SamplerUnits[i] = i;
   }

   return prog;
}


/**
 * Initialize a new fragment program object.
 */
struct gl_program *
_mesa_init_fragment_program( GLcontext *ctx, struct gl_fragment_program *prog,
                             GLenum target, GLuint id)
{
   if (prog)
      return _mesa_init_program_struct( ctx, &prog->Base, target, id );
   else
      return NULL;
}


/**
 * Initialize a new vertex program object.
 */
struct gl_program *
_mesa_init_vertex_program( GLcontext *ctx, struct gl_vertex_program *prog,
                           GLenum target, GLuint id)
{
   if (prog)
      return _mesa_init_program_struct( ctx, &prog->Base, target, id );
   else
      return NULL;
}


/**
 * Allocate and initialize a new fragment/vertex program object but
 * don't put it into the program hash table.  Called via
 * ctx->Driver.NewProgram.  May be overridden (ie. replaced) by a
 * device driver function to implement OO deriviation with additional
 * types not understood by this function.
 *
 * \param ctx  context
 * \param id   program id/number
 * \param target  program target/type
 * \return  pointer to new program object
 */
struct gl_program *
_mesa_new_program(GLcontext *ctx, GLenum target, GLuint id)
{
   struct gl_program *prog;
   switch (target) {
   case GL_VERTEX_PROGRAM_ARB: /* == GL_VERTEX_PROGRAM_NV */
   case GL_VERTEX_STATE_PROGRAM_NV:
      prog = _mesa_init_vertex_program(ctx, CALLOC_STRUCT(gl_vertex_program),
                                       target, id );
      break;
   case GL_FRAGMENT_PROGRAM_NV:
   case GL_FRAGMENT_PROGRAM_ARB:
      prog =_mesa_init_fragment_program(ctx,
                                         CALLOC_STRUCT(gl_fragment_program),
                                         target, id );
      break;
   default:
      _mesa_problem(ctx, "bad target in _mesa_new_program");
      prog = NULL;
   }
   return prog;
}


/**
 * Delete a program and remove it from the hash table, ignoring the
 * reference count.
 * Called via ctx->Driver.DeleteProgram.  May be wrapped (OO deriviation)
 * by a device driver function.
 */
void
_mesa_delete_program(GLcontext *ctx, struct gl_program *prog)
{
   (void) ctx;
   ASSERT(prog);
   ASSERT(prog->RefCount==0);

   if (prog == &_mesa_DummyProgram)
      return;

//   if (prog->String)
//      _mesa_free(prog->String);

   _mesa_free_instructions(prog->Instructions, prog->NumInstructions);

   if (prog->Parameters) {
      _mesa_free_parameter_list(prog->Parameters);
   }
   if (prog->Varying) {
      _mesa_free_parameter_list(prog->Varying);
   }
   if (prog->Attributes) {
      _mesa_free_parameter_list(prog->Attributes);
   }

   /* XXX this is a little ugly */
//   if (prog->Target == GL_VERTEX_PROGRAM_ARB) {
//      struct gl_vertex_program *vprog = (struct gl_vertex_program *) prog;
//      if (vprog->TnlData)
//         _mesa_free(vprog->TnlData);
//   }

   _mesa_free(prog);
}


/**
 * Reference counting for vertex/fragment programs
 */
void
_mesa_reference_program(GLcontext *ctx,
                        struct gl_program **ptr,
                        struct gl_program *prog)
{
   assert(ptr);
   if (*ptr && prog) {
      /* sanity check */
      if ((*ptr)->Target == GL_VERTEX_PROGRAM_ARB)
         ASSERT(prog->Target == GL_VERTEX_PROGRAM_ARB);
      else if ((*ptr)->Target == GL_FRAGMENT_PROGRAM_ARB)
         ASSERT(prog->Target == GL_FRAGMENT_PROGRAM_ARB ||
                prog->Target == GL_FRAGMENT_PROGRAM_NV);
   }
   if (*ptr == prog) {
      return;  /* no change */
   }
   if (*ptr) {
      GLboolean deleteFlag;

      /*_glthread_LOCK_MUTEX((*ptr)->Mutex);*/
#if 0
      printf("Program %p ID=%u Target=%s  Refcount-- to %d\n",
             *ptr, (*ptr)->Id,
             ((*ptr)->Target == GL_VERTEX_PROGRAM_ARB ? "VP" : "FP"),
             (*ptr)->RefCount - 1);
#endif
      ASSERT((*ptr)->RefCount > 0);
      (*ptr)->RefCount--;

      deleteFlag = ((*ptr)->RefCount == 0);
      /*_glthread_UNLOCK_MUTEX((*ptr)->Mutex);*/

      if (deleteFlag) {
         ASSERT(ctx);
         _mesa_delete_program(ctx, *ptr);
      }

      *ptr = NULL;
   }

   assert(!*ptr);
   if (prog) {
      /*_glthread_LOCK_MUTEX(prog->Mutex);*/
      prog->RefCount++;
#if 0
      printf("Program %p ID=%u Target=%s  Refcount++ to %d\n",
             prog, prog->Id,
             (prog->Target == GL_VERTEX_PROGRAM_ARB ? "VP" : "FP"),
             prog->RefCount);
#endif
      /*_glthread_UNLOCK_MUTEX(prog->Mutex);*/
   }

   *ptr = prog;
}


/**
 * Insert 'count' NOP instructions at 'start' in the given program.
 * Adjust branch targets accordingly.
 */
GLboolean
_mesa_insert_instructions(struct gl_program *prog, GLuint start, GLuint count)
{
   const GLuint origLen = prog->NumInstructions;
   const GLuint newLen = origLen + count;
   struct prog_instruction *newInst;
   GLuint i;

   /* adjust branches */
   for (i = 0; i < prog->NumInstructions; i++) {
      struct prog_instruction *inst = prog->Instructions + i;
      if (inst->BranchTarget > 0) {
         if ((GLuint)inst->BranchTarget >= start) {
            inst->BranchTarget += count;
         }
      }
   }

   /* Alloc storage for new instructions */
   newInst = _mesa_alloc_instructions(newLen);
   if (!newInst) {
      return GL_FALSE;
   }

   /* Copy 'start' instructions into new instruction buffer */
   _mesa_copy_instructions(newInst, prog->Instructions, start);

   /* init the new instructions */
   _mesa_init_instructions(newInst + start, count);

   /* Copy the remaining/tail instructions to new inst buffer */
   _mesa_copy_instructions(newInst + start + count,
                           prog->Instructions + start,
                           origLen - start);

   /* free old instructions */
   _mesa_free_instructions(prog->Instructions, origLen);

   /* install new instructions */
   prog->Instructions = newInst;
   prog->NumInstructions = newLen;

   return GL_TRUE;
}

/**
 * Delete 'count' instructions at 'start' in the given program.
 * Adjust branch targets accordingly.
 */
GLboolean
_mesa_delete_instructions(struct gl_program *prog, GLuint start, GLuint count)
{
   const GLuint origLen = prog->NumInstructions;
   const GLuint newLen = origLen - count;
   struct prog_instruction *newInst;
   GLuint i;

   /* adjust branches */
   for (i = 0; i < prog->NumInstructions; i++) {
      struct prog_instruction *inst = prog->Instructions + i;
      if (inst->BranchTarget > 0) {
         if (inst->BranchTarget > (GLint)start) {
            inst->BranchTarget -= count;
         }
      }
   }

   /* Alloc storage for new instructions */
   newInst = _mesa_alloc_instructions(newLen);
   if (!newInst) {
      return GL_FALSE;
   }

   /* Copy 'start' instructions into new instruction buffer */
   _mesa_copy_instructions(newInst, prog->Instructions, start);

   /* Copy the remaining/tail instructions to new inst buffer */
   _mesa_copy_instructions(newInst + start,
                           prog->Instructions + start + count,
                           newLen - start);

   /* free old instructions */
   _mesa_free_instructions(prog->Instructions, origLen);

   /* install new instructions */
   prog->Instructions = newInst;
   prog->NumInstructions = newLen;

   return GL_TRUE;
}


/**
 * Scan the given program to find a free register of the given type.
 * \param regFile - PROGRAM_INPUT, PROGRAM_OUTPUT or PROGRAM_TEMPORARY
 */
GLint
_mesa_find_free_register(const struct gl_program *prog, GLuint regFile)
{
   GLboolean used[MAX_PROGRAM_TEMPS];
   GLuint i, k;

   assert(regFile == PROGRAM_INPUT ||
          regFile == PROGRAM_OUTPUT ||
          regFile == PROGRAM_TEMPORARY);

   _mesa_memset(used, 0, sizeof(used));

   for (i = 0; i < prog->NumInstructions; i++) {
      const struct prog_instruction *inst = prog->Instructions + i;
      const GLuint n = _mesa_num_inst_src_regs(inst->Opcode);

      for (k = 0; k < n; k++) {
         if (inst->SrcReg[k].File == regFile) {
            used[inst->SrcReg[k].Index] = GL_TRUE;
         }
      }
   }

   for (i = 0; i < MAX_PROGRAM_TEMPS; i++) {
      if (!used[i])
         return i;
   }

   return -1;
}
