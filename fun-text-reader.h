// fun-text-reader.h -- Text-format input of an IR function
//
// Copyright © 2019  Miles Bader
//
// Author: Miles Bader <snogglethorpe@gmail.com>
// Created: 2019-11-03
//

#ifndef __FUN_TEXT_READER_H__
#define __FUN_TEXT_READER_H__

#include <string>
#include <unordered_map>


class ProgTextReader;

class Fun;


// A class for reading a text representations of a function.
//
class FunTextReader
{
public:

  FunTextReader (ProgTextReader &prog_Reader);


  // Return the text input source we're reading from.
  //
  SrcFileInput &input () const;


  // Read a text representation of a function, and return the new
  // function.
  //
  Fun *read ();


  // Read a block label.
  //
  BB *read_label ();

  // Read a register (which must exist).
  //
  Reg *read_lvalue_reg ();

  // Return the register (which must exist) called NAME
  //
  Reg *get_reg (const std::string &name);

  // Read a comma-separated list of lvalue registers, as for
  // FunTextReader::read_lvalue_reg, and return them as a vector.
  //
  std::vector<Reg *> read_lvalue_reg_list ();


  // Read either a register (which must exist), or a constant value
  // (which will be added to the current function if necessary), and
  // return the resulting register.
  //
  Reg *read_rvalue_reg ();

  // Read a comma-separated list of rvalue registers, as for
  // FunTextReader::read_rvalue_reg, and return them as a vector.
  //
  std::vector<Reg *> read_rvalue_reg_list ();


  // Return the block corresponding to the label LABEL.  If no such
  // label has yet been encountered, a new block is added and returned.
  //
  BB *label_block (const std::string &label);


private:

  // Parse the contents of a function.
  //
  void parse_fun ();

  // Clear any parsing state used while parsing a function.
  //
  void clear_state ();


  // Text reader for the program this is part of.
  //
  ProgTextReader &_prog_reader;


  // Function we're currently reading, or zero if none.
  //
  Fun *cur_fun = 0;

  // Block we're currently reading, or zero if none.
  //
  BB *cur_block = 0;


  std::unordered_map<std::string, BB *> labeled_blocks;

  std::unordered_map<std::string, Reg *> registers;
};


#endif // __FUN_TEXT_READER_H__
