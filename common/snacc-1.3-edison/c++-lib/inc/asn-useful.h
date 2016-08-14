//   NOTE: this is a machine generated file--editing not recommended
//
// asn-useful.h - class definitions for ASN.1 module ASN-USEFUL
//
//   This file was generated by snacc on Mon Dec  7 19:32:35 2009
//   UBC snacc by Mike Sample
//   A couple of enhancements made by IBM European Networking Center

#ifndef _asn_useful_h_
#define _asn_useful_h_


//------------------------------------------------------------------------------
// class declarations:

//namespace mibo {

class ObjectDescriptor;
class NumericString;
class PrintableString;
class TeletexString;
class T61String;
class VideotexString;
class IA5String;
class GraphicString;
class VisibleString;
class ISO646String;
class GeneralString;
class UTCTime;
class GeneralizedTime;
class EXTERNALChoice;
class EXTERNAL;

//------------------------------------------------------------------------------
// class definitions:

/* [UNIVERSAL 7] IMPLICIT OCTET STRING */
class ObjectDescriptor: public AsnOcts
{
public:
			ObjectDescriptor(): AsnOcts() {}
			ObjectDescriptor (const char *str): AsnOcts (str) {}
			ObjectDescriptor (const char *str, const size_t len): AsnOcts (str, len) {}
			ObjectDescriptor (const AsnOcts &o): AsnOcts (o) {}
  ObjectDescriptor		&operator = (const ObjectDescriptor &o)	{ ReSet (o); return *this; }
  ObjectDescriptor		&operator = (const char *str)	{ ReSet (str); return *this; }
  AsnType		*Clone() const;

  AsnLen		BEnc (BUF_TYPE b);
  void			BDec (BUF_TYPE b, AsnLen &bytesDecoded, ENV_TYPE env);
  int			BEncPdu (BUF_TYPE b, AsnLen &bytesEncoded);
  int			BDecPdu (BUF_TYPE b, AsnLen &bytesDecoded);

};


/* [UNIVERSAL 18] IMPLICIT OCTET STRING */
class NumericString: public AsnOcts
{
public:
			NumericString(): AsnOcts() {}
			NumericString (const char *str): AsnOcts (str) {}
			NumericString (const char *str, const size_t len): AsnOcts (str, len) {}
			NumericString (const AsnOcts &o): AsnOcts (o) {}
  NumericString		&operator = (const NumericString &o)	{ ReSet (o); return *this; }
  NumericString		&operator = (const char *str)	{ ReSet (str); return *this; }
  AsnType		*Clone() const;

  AsnLen		BEnc (BUF_TYPE b);
  void			BDec (BUF_TYPE b, AsnLen &bytesDecoded, ENV_TYPE env);
  int			BEncPdu (BUF_TYPE b, AsnLen &bytesEncoded);
  int			BDecPdu (BUF_TYPE b, AsnLen &bytesDecoded);

};


/* [UNIVERSAL 19] IMPLICIT OCTET STRING */
class PrintableString: public AsnOcts
{
public:
			PrintableString(): AsnOcts() {}
			PrintableString (const char *str): AsnOcts (str) {}
			PrintableString (const char *str, const size_t len): AsnOcts (str, len) {}
			PrintableString (const AsnOcts &o): AsnOcts (o) {}
  PrintableString		&operator = (const PrintableString &o)	{ ReSet (o); return *this; }
  PrintableString		&operator = (const char *str)	{ ReSet (str); return *this; }
  AsnType		*Clone() const;

  AsnLen		BEnc (BUF_TYPE b);
  void			BDec (BUF_TYPE b, AsnLen &bytesDecoded, ENV_TYPE env);
  int			BEncPdu (BUF_TYPE b, AsnLen &bytesEncoded);
  int			BDecPdu (BUF_TYPE b, AsnLen &bytesDecoded);

};


/* [UNIVERSAL 20] IMPLICIT OCTET STRING */
class TeletexString: public AsnOcts
{
public:
			TeletexString(): AsnOcts() {}
			TeletexString (const char *str): AsnOcts (str) {}
			TeletexString (const char *str, const size_t len): AsnOcts (str, len) {}
			TeletexString (const AsnOcts &o): AsnOcts (o) {}
  TeletexString		&operator = (const TeletexString &o)	{ ReSet (o); return *this; }
  TeletexString		&operator = (const char *str)	{ ReSet (str); return *this; }
  AsnType		*Clone() const;

  AsnLen		BEnc (BUF_TYPE b);
  void			BDec (BUF_TYPE b, AsnLen &bytesDecoded, ENV_TYPE env);
  int			BEncPdu (BUF_TYPE b, AsnLen &bytesEncoded);
  int			BDecPdu (BUF_TYPE b, AsnLen &bytesDecoded);

};


/* [UNIVERSAL 20] IMPLICIT OCTET STRING */
class T61String: public AsnOcts
{
public:
			T61String(): AsnOcts() {}
			T61String (const char *str): AsnOcts (str) {}
			T61String (const char *str, const size_t len): AsnOcts (str, len) {}
			T61String (const AsnOcts &o): AsnOcts (o) {}
  T61String		&operator = (const T61String &o)	{ ReSet (o); return *this; }
  T61String		&operator = (const char *str)	{ ReSet (str); return *this; }
  AsnType		*Clone() const;

  AsnLen		BEnc (BUF_TYPE b);
  void			BDec (BUF_TYPE b, AsnLen &bytesDecoded, ENV_TYPE env);
  int			BEncPdu (BUF_TYPE b, AsnLen &bytesEncoded);
  int			BDecPdu (BUF_TYPE b, AsnLen &bytesDecoded);

};


/* [UNIVERSAL 21] IMPLICIT OCTET STRING */
class VideotexString: public AsnOcts
{
public:
			VideotexString(): AsnOcts() {}
			VideotexString (const char *str): AsnOcts (str) {}
			VideotexString (const char *str, const size_t len): AsnOcts (str, len) {}
			VideotexString (const AsnOcts &o): AsnOcts (o) {}
  VideotexString		&operator = (const VideotexString &o)	{ ReSet (o); return *this; }
  VideotexString		&operator = (const char *str)	{ ReSet (str); return *this; }
  AsnType		*Clone() const;

  AsnLen		BEnc (BUF_TYPE b);
  void			BDec (BUF_TYPE b, AsnLen &bytesDecoded, ENV_TYPE env);
  int			BEncPdu (BUF_TYPE b, AsnLen &bytesEncoded);
  int			BDecPdu (BUF_TYPE b, AsnLen &bytesDecoded);

};


/* [UNIVERSAL 22] IMPLICIT OCTET STRING */
class IA5String: public AsnOcts
{
public:
			IA5String(): AsnOcts() {}
			IA5String (const char *str): AsnOcts (str) {}
			IA5String (const char *str, const size_t len): AsnOcts (str, len) {}
			IA5String (const AsnOcts &o): AsnOcts (o) {}
  IA5String		&operator = (const IA5String &o)	{ ReSet (o); return *this; }
  IA5String		&operator = (const char *str)	{ ReSet (str); return *this; }
  AsnType		*Clone() const;

  AsnLen		BEnc (BUF_TYPE b);
  void			BDec (BUF_TYPE b, AsnLen &bytesDecoded, ENV_TYPE env);
  int			BEncPdu (BUF_TYPE b, AsnLen &bytesEncoded);
  int			BDecPdu (BUF_TYPE b, AsnLen &bytesDecoded);

};


/* [UNIVERSAL 25] IMPLICIT OCTET STRING */
class GraphicString: public AsnOcts
{
public:
			GraphicString(): AsnOcts() {}
			GraphicString (const char *str): AsnOcts (str) {}
			GraphicString (const char *str, const size_t len): AsnOcts (str, len) {}
			GraphicString (const AsnOcts &o): AsnOcts (o) {}
  GraphicString		&operator = (const GraphicString &o)	{ ReSet (o); return *this; }
  GraphicString		&operator = (const char *str)	{ ReSet (str); return *this; }
  AsnType		*Clone() const;

  AsnLen		BEnc (BUF_TYPE b);
  void			BDec (BUF_TYPE b, AsnLen &bytesDecoded, ENV_TYPE env);
  int			BEncPdu (BUF_TYPE b, AsnLen &bytesEncoded);
  int			BDecPdu (BUF_TYPE b, AsnLen &bytesDecoded);

};


/* [UNIVERSAL 26] IMPLICIT OCTET STRING */
class VisibleString: public AsnOcts
{
public:
			VisibleString(): AsnOcts() {}
			VisibleString (const char *str): AsnOcts (str) {}
			VisibleString (const char *str, const size_t len): AsnOcts (str, len) {}
			VisibleString (const AsnOcts &o): AsnOcts (o) {}
  VisibleString		&operator = (const VisibleString &o)	{ ReSet (o); return *this; }
  VisibleString		&operator = (const char *str)	{ ReSet (str); return *this; }
  AsnType		*Clone() const;

  AsnLen		BEnc (BUF_TYPE b);
  void			BDec (BUF_TYPE b, AsnLen &bytesDecoded, ENV_TYPE env);
  int			BEncPdu (BUF_TYPE b, AsnLen &bytesEncoded);
  int			BDecPdu (BUF_TYPE b, AsnLen &bytesDecoded);

};


/* [UNIVERSAL 26] IMPLICIT OCTET STRING */
class ISO646String: public AsnOcts
{
public:
			ISO646String(): AsnOcts() {}
			ISO646String (const char *str): AsnOcts (str) {}
			ISO646String (const char *str, const size_t len): AsnOcts (str, len) {}
			ISO646String (const AsnOcts &o): AsnOcts (o) {}
  ISO646String		&operator = (const ISO646String &o)	{ ReSet (o); return *this; }
  ISO646String		&operator = (const char *str)	{ ReSet (str); return *this; }
  AsnType		*Clone() const;

  AsnLen		BEnc (BUF_TYPE b);
  void			BDec (BUF_TYPE b, AsnLen &bytesDecoded, ENV_TYPE env);
  int			BEncPdu (BUF_TYPE b, AsnLen &bytesEncoded);
  int			BDecPdu (BUF_TYPE b, AsnLen &bytesDecoded);

};


/* [UNIVERSAL 27] IMPLICIT OCTET STRING */
class GeneralString: public AsnOcts
{
public:
			GeneralString(): AsnOcts() {}
			GeneralString (const char *str): AsnOcts (str) {}
			GeneralString (const char *str, const size_t len): AsnOcts (str, len) {}
			GeneralString (const AsnOcts &o): AsnOcts (o) {}
  GeneralString		&operator = (const GeneralString &o)	{ ReSet (o); return *this; }
  GeneralString		&operator = (const char *str)	{ ReSet (str); return *this; }
  AsnType		*Clone() const;

  AsnLen		BEnc (BUF_TYPE b);
  void			BDec (BUF_TYPE b, AsnLen &bytesDecoded, ENV_TYPE env);
  int			BEncPdu (BUF_TYPE b, AsnLen &bytesEncoded);
  int			BDecPdu (BUF_TYPE b, AsnLen &bytesDecoded);

};


/* [UNIVERSAL 23] IMPLICIT OCTET STRING */
class UTCTime: public AsnOcts
{
public:
			UTCTime(): AsnOcts() {}
			UTCTime (const char *str): AsnOcts (str) {}
			UTCTime (const char *str, const size_t len): AsnOcts (str, len) {}
			UTCTime (const AsnOcts &o): AsnOcts (o) {}
  UTCTime		&operator = (const UTCTime &o)	{ ReSet (o); return *this; }
  UTCTime		&operator = (const char *str)	{ ReSet (str); return *this; }
  AsnType		*Clone() const;

  AsnLen		BEnc (BUF_TYPE b);
  void			BDec (BUF_TYPE b, AsnLen &bytesDecoded, ENV_TYPE env);
  int			BEncPdu (BUF_TYPE b, AsnLen &bytesEncoded);
  int			BDecPdu (BUF_TYPE b, AsnLen &bytesDecoded);

};


/* [UNIVERSAL 24] IMPLICIT OCTET STRING */
class GeneralizedTime: public AsnOcts
{
public:
			GeneralizedTime(): AsnOcts() {}
			GeneralizedTime (const char *str): AsnOcts (str) {}
			GeneralizedTime (const char *str, const size_t len): AsnOcts (str, len) {}
			GeneralizedTime (const AsnOcts &o): AsnOcts (o) {}
  GeneralizedTime		&operator = (const GeneralizedTime &o)	{ ReSet (o); return *this; }
  GeneralizedTime		&operator = (const char *str)	{ ReSet (str); return *this; }
  AsnType		*Clone() const;

  AsnLen		BEnc (BUF_TYPE b);
  void			BDec (BUF_TYPE b, AsnLen &bytesDecoded, ENV_TYPE env);
  int			BEncPdu (BUF_TYPE b, AsnLen &bytesEncoded);
  int			BDecPdu (BUF_TYPE b, AsnLen &bytesDecoded);

};


class EXTERNALChoice: public AsnType
{
public:
  enum ChoiceIdEnum
  {
     single_ASN1_typeCid = 0,
     octet_alignedCid = 1,
     arbitraryCid = 2
  };

  enum ChoiceIdEnum	choiceId;
  union
  {
     AsnOcts		*single_ASN1_type;
     AsnOcts		*octet_aligned;
     AsnBits		*arbitrary;
  };


			EXTERNALChoice();
			EXTERNALChoice (const EXTERNALChoice &);
			~EXTERNALChoice();

  AsnType		*Clone() const;

  EXTERNALChoice		&operator = (const EXTERNALChoice &);
  AsnLen		BEncContent (BUF_TYPE b);
  void			BDecContent (BUF_TYPE b, AsnTag tag, AsnLen elmtLen, AsnLen &bytesDecoded, ENV_TYPE env);
  AsnLen		BEnc (BUF_TYPE b);
  void			BDec (BUF_TYPE b, AsnLen &bytesDecoded, ENV_TYPE env);
  int			BEncPdu (BUF_TYPE b, AsnLen &bytesEncoded);
  int			BDecPdu (BUF_TYPE b, AsnLen &bytesDecoded);

  void			Print (ostream &os) const;
};


class EXTERNAL: public AsnType
{
public:
  AsnOid		*direct_reference;
  AsnInt		*indirect_reference;
  ObjectDescriptor		*data_value_descriptor;
  EXTERNALChoice		*encoding;

			EXTERNAL();
			EXTERNAL (const EXTERNAL &);
			~EXTERNAL();
  AsnType		*Clone() const;

  EXTERNAL		&operator = (const EXTERNAL &);
  AsnLen		BEncContent (BUF_TYPE b);
  void			BDecContent (BUF_TYPE b, AsnTag tag, AsnLen elmtLen, AsnLen &bytesDecoded, ENV_TYPE env);

  AsnLen		BEnc (BUF_TYPE b);
  void			BDec (BUF_TYPE b, AsnLen &bytesDecoded, ENV_TYPE env);
  int			BEncPdu (BUF_TYPE b, AsnLen &bytesEncoded);
  int			BDecPdu (BUF_TYPE b, AsnLen &bytesDecoded);

  void		Print (ostream &os) const;
};


//------------------------------------------------------------------------------
// externs for value defs

//------------------------------------------------------------------------------
//}/* end of namespace mibo */


#endif /* conditional include of asn-useful.h */
