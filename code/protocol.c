/* pool.c: PROTOCOL IMPLEMENTATION
 *
 * $Id$
 * Copyright (c) 2001-2016 Ravenbrook Limited.  See end of file for license.
 *
 * See design.mps.protocol.
 */

#include "mpm.h"
#include "protocol.h"
#include "classdef.h"

SRCID(protocol, "$Id$");


/* InstClassCheck -- check a protocol class */

Bool InstClassCheck(InstClass class)
{
  ClassLevel i;
  CHECKS(InstClass, class);
  CHECKL(class->name != NULL);
  CHECKL(class->level < ClassDEPTH);
  for (i = 0; i <= class->level; ++i) {
    CHECKL(class->display[i] != 0);
    CHECKL(class->display[i] < ClassIdLIMIT);
  }
  for (i = class->level + 1; i < ClassDEPTH; ++i) {
    CHECKL(class->display[i] == 0);
  }
  return TRUE;
}


/* InstInit -- initialize a protocol instance
 *
 * Initialisation makes the instance valid, so that it will pass
 * InstCheck, and the instance can be specialized to be a member of a
 * subclass.
 */

void InstInit(Inst inst)
{
  AVER(inst != NULL); /* FIXME: express intention here */
  inst->class = CLASS(Inst);
  AVERC(Inst, inst);
}


/* InstFinish -- finish a protocol instance
 *
 * Finishing makes the instance invalid, so that it will fail
 * InstCheck and can't be used.
 *
 * FIXME: It would be nice if we could use a recognizable value here,
 * such as a pointer to a static invalid class.
 */

static InstClassStruct invalidClassStruct = {
  /* .sig = */        SigInvalid,
  /* .name = */       "Invalid",
  /* .superclass = */ &invalidClassStruct,
  /* .level = */      ClassIdInvalid,
  /* .display = */    {ClassIdInvalid}
};
  
void InstFinish(Inst inst)
{
  AVERC(Inst, inst);
  inst->class = &invalidClassStruct;
}


/* InstCheck -- check a protocol instance */

Bool InstCheck(Inst inst)
{
  CHECKD(InstClass, inst->class);
  return TRUE;
}


/* The class definition for the root of the hierarchy */

DEFINE_CLASS(Inst, Inst, theClass)
{
  ClassLevel i;
  theClass->sig = InstClassSig;
  theClass->name = "Inst";
  theClass->superclass = theClass;
  for (i = 0; i < ClassDEPTH; ++i)
    theClass->display[i] = 0;
  theClass->level = 0;
  theClass->display[theClass->level] = ClassIdInst;
  AVERT(InstClass, theClass);
}


/* ClassOf* -- get the class of an instance */

#define CLASS_DEFINE_CLASSOF(prefix, ident, kind, super) \
  CLASS_TYPE(kind) (prefix ## ident)(struct INST_STRUCT(ident) *inst) \
  { \
    /* extern Bool INST_CHECK(ident)(struct INST_STRUCT(ident) *inst); */ \
    /* AVERC(ident, inst); */ \
    return (CLASS_TYPE(kind))CouldBeA(Inst, inst)->class; \
  }

CLASSES(CLASS_DEFINE_CLASSOF, ClassOf)


/* SetClassOf -- set the class of an instance */

#define CLASS_DEFINE_SETCLASSOF(prefix, ident, kind, super) \
  void (prefix ## ident)(struct INST_STRUCT(ident) *inst, CLASS_TYPE(kind) class) \
  { \
    AVERT(CLASS_TYPE(kind), class); \
    CouldBeA(Inst, inst)->class = (InstClass)class; \
  }

CLASSES(CLASS_DEFINE_SETCLASSOF, SetClassOf)


/* C. COPYRIGHT AND LICENSE
 *
 * Copyright (C) 2001-2016 Ravenbrook Limited <http://www.ravenbrook.com/>.
 * All rights reserved.  This is an open source license.  Contact
 * Ravenbrook for commercial licensing options.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * 3. Redistributions in any form must be accompanied by information on how
 * to obtain complete source code for this software and any accompanying
 * software that uses this software.  The source code must either be
 * included in the distribution or be available for no more than the cost
 * of distribution plus a nominal fee, and must be freely redistributable
 * under reasonable conditions.  For an executable file, complete source
 * code means the source code for all modules it contains. It does not
 * include source code for modules or files that typically accompany the
 * major components of the operating system on which the executable file
 * runs.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, OR NON-INFRINGEMENT, ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS AND CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
