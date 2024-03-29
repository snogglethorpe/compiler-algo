// prog-text-reader.cc -- Text-format input of an IR program
//
// Copyright © 2019  Miles Bader
//
// Author: Miles Bader <snogglethorpe@gmail.com>
// Created: 2019-11-14
//

#include <stdexcept>

#include "prog.h"

#include "src-file-input.h"

#include "prog-text-reader.h"


ProgTextReader::ProgTextReader (SrcFileInput &inp)
  : _inp (inp), _fun_reader (*this)
{
}


// Read a text representation of a program, and return the new program..
//
Prog *
ProgTextReader::read ()
{
  // Cannot be called recursively.
  //
  if (cur_prog)
    throw std::runtime_error ("Recursive call to ProgTextReader::read");

  Prog *prog = new Prog ();

  cur_prog = prog;

  do
    {
      if (! _inp.at_eol ())
	{
	  _inp.expect ("fun");
	  std::string fun_name = _inp.read_id ();

	  Fun *fun = _fun_reader.read ();

	  prog->add_fun (fun_name, fun);
	}
    }
  while (_inp.read_new_line ());

  cur_prog = 0;

  return prog;
}
