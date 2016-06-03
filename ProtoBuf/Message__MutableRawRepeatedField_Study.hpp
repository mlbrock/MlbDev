// ////////////////////////////////////////////////////////////////////////////
// From message.h
class LIBPROTOBUF_EXPORT Reflection {
   // ...
 protected:
  // Obtain a pointer to a Repeated Field Structure and do some type checking:
  //   on field->cpp_type(),
  //   on field->field_option().ctype() (if ctype >= 0)
  //   of field->message_type() (if message_type != NULL).
  // We use 1 routine rather than 4 (const vs mutable) x (scalar vs pointer).
  virtual void* MutableRawRepeatedField(
      Message* message, const FieldDescriptor* field, FieldDescriptor::CppType,
      int ctype, const Descriptor* message_type) const = 0;

 private:
  // Special version for specialized implementations of string.  We can't call
  // MutableRawRepeatedField directly here because we don't have access to
  // FieldOptions::* which are defined in descriptor.pb.h.  Including that
  // file here is not possible because it would cause a circular include cycle.
  void* MutableRawRepeatedString(
      Message* message, const FieldDescriptor* field, bool is_string) const;

   // ...
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// From message.h
#define DECLARE_GET_REPEATED_FIELD(TYPE)                         \
template<>                                                       \
LIBPROTOBUF_EXPORT                                               \
const RepeatedField<TYPE>& Reflection::GetRepeatedField<TYPE>(   \
    const Message& message, const FieldDescriptor* field) const; \
                                                                 \
template<>                                                       \
RepeatedField<TYPE>* Reflection::MutableRepeatedField<TYPE>(     \
    Message* message, const FieldDescriptor* field) const;

DECLARE_GET_REPEATED_FIELD(int32)
DECLARE_GET_REPEATED_FIELD(int64)
DECLARE_GET_REPEATED_FIELD(uint32)
DECLARE_GET_REPEATED_FIELD(uint64)
DECLARE_GET_REPEATED_FIELD(float)
DECLARE_GET_REPEATED_FIELD(double)
DECLARE_GET_REPEATED_FIELD(bool)

#undef DECLARE_GET_REPEATED_FIELD

// =============================================================================
// Implementation details for {Get,Mutable}RawRepeatedPtrField.  We provide
// specializations for <string>, <StringPieceField> and <Message> and handle
// everything else with the default template which will match any type having
// a method with signature "static const google::protobuf::Descriptor* descriptor()".
// Such a type presumably is a descendant of google::protobuf::Message.

template<>
inline const RepeatedPtrField<string>& Reflection::GetRepeatedPtrField<string>(
    const Message& message, const FieldDescriptor* field) const {
  return *static_cast<RepeatedPtrField<string>* >(
      MutableRawRepeatedString(const_cast<Message*>(&message), field, true));
}

template<>
inline RepeatedPtrField<string>* Reflection::MutableRepeatedPtrField<string>(
    Message* message, const FieldDescriptor* field) const {
  return static_cast<RepeatedPtrField<string>* >(
      MutableRawRepeatedString(message, field, true));
}


// -----

template<>
inline const RepeatedPtrField<Message>& Reflection::GetRepeatedPtrField(
    const Message& message, const FieldDescriptor* field) const {
  return *static_cast<RepeatedPtrField<Message>* >(
      MutableRawRepeatedField(const_cast<Message*>(&message), field,
          FieldDescriptor::CPPTYPE_MESSAGE, -1,
          NULL));
}

template<>
inline RepeatedPtrField<Message>* Reflection::MutableRepeatedPtrField(
    Message* message, const FieldDescriptor* field) const {
  return static_cast<RepeatedPtrField<Message>* >(
      MutableRawRepeatedField(message, field,
          FieldDescriptor::CPPTYPE_MESSAGE, -1,
          NULL));
}

template<typename PB>
inline const RepeatedPtrField<PB>& Reflection::GetRepeatedPtrField(
    const Message& message, const FieldDescriptor* field) const {
  return *static_cast<RepeatedPtrField<PB>* >(
      MutableRawRepeatedField(const_cast<Message*>(&message), field,
          FieldDescriptor::CPPTYPE_MESSAGE, -1,
          PB::default_instance().GetDescriptor()));
}

template<typename PB>
inline RepeatedPtrField<PB>* Reflection::MutableRepeatedPtrField(
    Message* message, const FieldDescriptor* field) const {
  return static_cast<RepeatedPtrField<PB>* >(
      MutableRawRepeatedField(message, field,
          FieldDescriptor::CPPTYPE_MESSAGE, -1,
          PB::default_instance().GetDescriptor()));
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
http://stackoverflow.com/questions/28981192/protocol-buffer-getrepeatedfield-reflection-code-optimization
// ----------------------------------------------------------------------------
/*
The Correct Answer

The right answer here is that instead of using
Reflection::GetRepeatedPtrField<T>(), you should use
Reflection::GetRepeatedMessage() to get a generic const Message& for each
element of the repeated field. Unfortunately you will need to call this
method once for each element (use Reflection::FieldSize() to find the size).

On each Message, you can then use Message::GetDescriptor(), look up the field
named test_field, then Message::GetReflection() and use that to read the value
of that field. As an optimization, you can safely assume that all messages in
the same repeated field have the same Descriptor and Reflection objects, so
you only need to get these objects and look up the FieldDescriptor once for
the whole array.

Something like (not tested):

int size = pReflection->FieldSize(*BATTLE_DATA, pMessageField);
Reflection* pInnerReflection = NULL;
FieldDescriptor* pTestFieldDesc = NULL;
for (int i = 0; i < size; i++) {
  const Message& msg = pReflection->GetRepeatedMessage(
      *BATTLE_DATA, pMessageField, i);
  if (pInnerReflection == NULL) {
    pInnerReflection = msg.GetReflection();
    pTestFieldDesc = msg.GetDescriptor()
        ->FindFieldByName("test_field");
  }
  int testField = pInnerReflection->GetInt32(msg, pTestFieldDesc);
  // ...
}

A Fun But Bad Answer

This second solution will perform slightly better and look a little prettier.
Unfortunately, it is technically undefined behavior. It will work in practice
on all compilers but probably you shouldn't do it unless you have a performance
problem. I include it here for fun, and because you'll probably figure it out
anyway so it's good to know about why it's bad.

You could call GetRepeatedPtrField<google::protobuf::Message>(...) to get a
RepeatedPtrField of generic Message objects. You can then obtain the descriptor
and reflection as with the first solution, but you no longer need to call
Reflection::GetMessage (a virtual call) to read each message. (You do still
need to invoke reflection to read test_field, though.)

This is technically undefined behavior because it assumes a few things:

That RepeatedPtrField<T> has the same layout for all T. This is in fact true
because RepeatedPtrField<T> extends RepeatedPtrFieldBase and does not add any
new fields, but the C++ standard doesn't require that this be the case.

That upcasting a specific message type pointer (like AInfo*) to Message* does
not change the bits of the pointer. In practice, this is true on all compilers
as long as no multiple inheritance is in play, and Protobufs does not use
multiple inheritance (the Google C++ style guide even prohibits it).

That the compiler will not optimize based on strict aliasing rules in such a
way that it ends up reordering accesses to your RepeatedPtrField<Message> vs.
other accesses to RepeatedPtrField<AInfo> (or whichever type) occurring
elsewhere in the program. In practice, it's unreasonable to imagine any
compiler actually doing this for this use case, but it's technically allowed
by C++.
*/
// ////////////////////////////////////////////////////////////////////////////


