// bb.cc -- IR basic blocks
//
// Copyright © 2019  Miles Bader
//
// Author: Miles Bader <snogglethorpe@gmail.com>
// Created: 2019-10-28
//

#include "check-assertion.h"

#include "bb.h"
#include "insn.h"
#include "remove-one.h"

#include "fun.h"


// Make a new block in function FUN.
//
BB::BB (Fun *fun)
  : _fun (fun)
{
  fun->add_block (this);
}


// Destructor; tries to cleanly remove the block from the flow
// graph.
//
BB::~BB ()
{
  while (! _insns.empty ())
    delete _insns.front ();

  BB *fall_through = _fall_through;

  set_fall_through (0);

  // In the case of a self-loop, we just zero any incoming edges
  // instead of removing this block from them.
  //
  if (fall_through == this)
    fall_through = 0;

  // Remove this block from each predecessor.
  //
  while (! _preds.empty ())
    _preds.front ()->change_successor (this, fall_through);

  check_assertion (_preds.empty (), "Destroyed block has remaining predecessors");
  check_assertion (_succs.empty (), "Destroyed block has remaining successors");

  Fun *fun = _fun;
  if (fun)
    {
      _fun = 0;
      fun->remove_block (this);
    }

  remove_from_dominator_tree (&BB::fwd_dom_tree_node);
  remove_from_dominator_tree (&BB::bwd_dom_tree_node);
}


// Mark dominator / post-dominator information in this
// block's function as out of date.
//
void
BB::invalidate_dominators ()
{
  if (_fun) _fun->invalidate_dominators ();
}
void
BB::invalidate_post_dominators ()
{
  if (_fun) _fun->invalidate_post_dominators ();
}


// Add the instruction INSN to the end of this block.
//
void
BB::add_insn (Insn *insn)
{
  BB *old_block = insn->block ();

  if (old_block)
    old_block->remove_insn (insn);

  _insns.push_back (insn);

  insn->set_block (this);
}

// Add the instruction INSN to the end of this block, but before any
// branch instructio if there is one.
//
void
BB::add_insn_before_branch (Insn *insn)
{
  BB *old_block = insn->block ();

  if (old_block)
    old_block->remove_insn (insn);

  // If the last instruction is a branch, insert before it, otherwise
  // just insert at the end.
  //
  if (_insns.empty () || !_insns.back()->is_branch_insn ())
    {
      // Just insert at end.

    _insns.push_back (insn);
    }
  else
    {
      // Insert before last insn.

      auto insert_before = _insns.end ();

      --insert_before;

      _insns.insert (insert_before, insn);
    }

  insn->set_block (this);
}

// Add the instruction INSN to the beginning of this block.
//
void
BB::prepend_insn (Insn *insn)
{
  BB *old_block = insn->block ();

  if (old_block)
    old_block->remove_insn (insn);

  _insns.push_front (insn);

  insn->set_block (this);
}

// Remove the instruction INSN from this block.
//
void
BB::remove_insn (Insn *insn)
{
  _insns.remove (insn);
  insn->set_block (0);
}


// Set the block to which control-flow in this block goes if
// execution runs off the end of it to BLOCK.  BLOCK may be zero.
//
void
BB::set_fall_through (BB *block)
{
  if (block != _fall_through)
    {
      if (_fall_through)
	remove_successor (_fall_through);

      if (block)
	add_successor (block);

      _fall_through = block;
    }
}


// Add a new control flow edge reflecting that SUCC is now a
// successor of this block.
///
void
BB::add_successor (BB *succ)
{
  _succs.push_back (succ);
  succ->_preds.push_back (this);

  invalidate_dominators ();
  invalidate_post_dominators ();
}

// Remove a control flow edge between this block and the successor
// block SUCC.  If there are multiple control flow edges between
// this block and SUCC, only a single edge is removed.
//
void
BB::remove_successor (BB *succ)
{
  // We can't use std::list<T>::remove, because it will remove all
  // matching entries, and we only want to remove one.

  remove_one (succ, _succs);
  remove_one (this, succ->_preds);

  invalidate_dominators ();
  invalidate_post_dominators ();
}


// Replace FROM in the successors of this block with TO.  TO may be
// NULL, in which case the target becomes undefined.
//
void
BB::change_successor (BB *from, BB *to)
{
  if (from == _fall_through)
    set_fall_through (to);

  if (! _insns.empty ())
    _insns.back ()->change_branch_target (from, to);
}
