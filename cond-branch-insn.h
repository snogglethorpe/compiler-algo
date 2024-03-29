// cond-branch-insn.h -- IR conditional branch instructions
//
// Copyright © 2019  Miles Bader
//
// Author: Miles Bader <snogglethorpe@gmail.com>
// Created: 2019-10-31
//

#ifndef __COND_BRANCH_INSN_H__
#define __COND_BRANCH_INSN_H__


#include "insn.h"


class Reg;


// IR conditional branch instruction, representing a conditional
// control-flow change.
//
class CondBranchInsn : public Insn
{
public:

  // Make a new conditional-branch instruction, which transfers
  // control to TARGET if COND contains a non-zero value, optionally
  // appending it to block BLOCK.
  //
  CondBranchInsn (Reg *cond = 0, BB *target = 0, BB *block = 0);

  virtual ~CondBranchInsn ();

  // Do instruction-specific setup after this instruction has been
  // added to block BLOCK.  At the point this is called, this
  // instruction is in BLOCK's instruction list, but nothing else has
  // been done.
  //
  virtual void set_block (BB *block);

  // Return true if this is a branch instruction, that is, if it may
  // affect control-flow.
  //
  virtual bool is_branch_insn () const { return true; }

  // Return true if this instruction has a visible side-effect other
  // than reading from its argument registers and writing to its
  // result registers.
  //
  virtual bool has_side_effect () const { return true; }


  // For each branch target block in this insn which points to FROM,
  // change the target to TO.  TO may be NULL, in which case the
  // target becomes undefined.
  //
  virtual void change_branch_target (BB *from, BB *to);


  // Return the block to which this conditional branch instruction may
  // transfer control [if -some- condition is true].
  //
  BB *target () const { return _target; }

  // Set the block to which this conditional branch instruction may
  // transfer control [if -some- condition is true].
  //
  void set_target (BB *target);


  // Return the register that determines whether this conditional
  // branch instruction does a control transfer or not.  If the
  // designated register contains a non-zero value, control is
  // transferred, otherwise it is not.
  //
  Reg *condition () const { return args ()[0]; }


private:

  // Block to which this conditional branch instruction may transfer
  // control [if -some- condition is true].
  //
  BB *_target = 0;
};


#endif // __COND_BRANCH_INSN_H__
