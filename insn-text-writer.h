#ifndef __INSN_TEXT_WRITER_H__
#define __INSN_TEXT_WRITER_H__

#include <unordered_map>
#include <typeinfo>
#include <typeindex>


class Insn;
class FunTextWriter;


// A class for outputting text representations of isns.
//
class InsnTextWriter
{
public:

  InsnTextWriter (FunTextWriter &fun_writer);

  // Write a text representation of INSN.
  //
  void write (Insn *insn);


private:

  // Text writer for the function we're associated with.
  //
  FunTextWriter &fun_writer;


private:
  // Text writer methods for various insn types.

  void write_cond_branch (Insn *insn);


private:
  // Text writer method dispatch machinery.  This is all global, and so
  // initialized only once.

  typedef void (InsnTextWriter::*WriteMeth) (Insn *insn);

  static std::unordered_map<std::type_index, WriteMeth> write_meths;

  // Populate the WRITE_METHS data member.
  //
  static void setup_write_meths ();

  // Called when there's a mixup and write method that expected an insn
  // of type WANTED is called on the insn INSN which is not of that
  // type.  Raises an exception.
  //
  void invalid_write_method (Insn *insn, const char *wanted);
};


#endif // __INSN_TEXT_WRITER_H__