#ifndef SignalTypeAST_h
#define SignalTypeAST_h

#include "AtlasAST.h"
#include "InitList.h"
#include "NounsModifiersDimensions.h"
#include "TimerObject.h"

class SignalTypeAST : public AST {
  public:
    SignalTypeAST(ANTLRTokenPtr p = 0, TheType t = UndefinedTypeValue);
    virtual ~SignalTypeAST();

    virtual AST    *eval(AST *a = 0);
    virtual AST    *assign(AST *a);
    virtual AST    *data(AST *a = 0);
    virtual Long    compare(AST *o) const;
    virtual AST    *check(AST *a = 0);
    virtual TheType getType(AST *a = 0) const;
    virtual AST    *Accept(ASTVisitor &);

  private:
    TheType _storage;
};

class AnalogType : public SignalTypeAST {
  public:
    AnalogType(ANTLRTokenPtr p = 0);

  private:
};

class DoSimulStructureType : public SignalTypeAST {
  public:
    DoSimulStructureType(ANTLRTokenPtr p = 0);

  private:
};

class DoSimulStatementType : public SignalTypeAST {
  public:
    DoSimulStatementType(ANTLRTokenPtr p = 0);

  private:
};

class DoSimulBodyType : public SignalTypeAST {
  public:
    DoSimulBodyType(ANTLRTokenPtr p = 0);

  private:
};

class DoSimulEndDoType : public SignalTypeAST {
  public:
    DoSimulEndDoType(ANTLRTokenPtr p = 0);

  private:
};

class NounType : public SignalTypeAST {
  public:
    NounType(ANTLRTokenPtr p, NounEntry *ne);
    virtual AST      *eval(AST *a = 0);
    virtual AST      *check(AST *a = 0);
    virtual AST      *init(AST *a = 0);
    virtual Long      compare(AST *o) const;
    virtual RWCString getName() const;
    virtual AST      *data(AST *a = 0);
    virtual astream  &operator<<(astream &s);

    NounEntry *_nounEntry;
};

class StatementCharacteristics : public SignalTypeAST {
  public:
    StatementCharacteristics(NounEntry *ne);

  private:
    NounEntry *_nounEntry;
};

class StatementCharacteristic : public SignalTypeAST {
  public:
    StatementCharacteristic(ModifierEntry *me);
    virtual AST      *eval(AST *a = 0);
    virtual TheType   getType(AST *a = 0) const;
    virtual Long      compare(AST *o) const;
    virtual AST      *check(AST *a = 0);
    virtual RWCString getName() const;

  private:
};

class ModifierType : public SignalTypeAST {
  public:
    ModifierType(ANTLRTokenPtr p, ModifierEntry *me, class AtlasParser *parser = 0);
    virtual RWCString getName() const;
    virtual Long      compare(AST *o) const;
    virtual AST      *check(AST *a = 0);
    virtual AST      *init(AST *a = 0);
    virtual AST      *data(AST *a = 0);
    virtual void      print(AST *a = 0) const;

  protected:
    ModifierEntry *m_modifierEntry;
    AtlasParser   *m_parser;
};

class ModifierListType : public SignalTypeAST {
  public:
    ModifierListType(ASTList *l);

    virtual AST     *check(AST *a);
    virtual Long     compare(AST *o) const;
    virtual AST     *data(AST *a = 0);
    virtual AST     *init(AST *a = 0);
    virtual void     print(AST *a = 0) const;
    virtual AST     *assign(AST *a);
    virtual AST     *Accept(ASTVisitor &);
    virtual astream &operator<<(astream &s);

    ASTList::iterator begin() { return _modifierList->begin(); }
    ASTList::iterator end() { return _modifierList->end(); }

    ASTList::const_iterator begin() const { return _modifierList->begin(); }
    ASTList::const_iterator end() const { return _modifierList->end(); }

  public:
    ASTList *_modifierList;
};

class MeasuredCharacteristicsType : public ModifierListType {
  public:
    MeasuredCharacteristicsType(ASTList *l);

    // virtual AST *	assign	( AST * a    );

  public:
    // ASTList * _modifierList;
};

class ReqModifierListType : public ModifierListType {
  public:
    ReqModifierListType(ASTList *cntl, ASTList *capl, ASTList *liml);

    virtual AST *check(AST *a);
    virtual Long compare(AST *o) const;
    virtual AST *data(AST *a = 0);
    virtual AST *init(AST *a = 0);
    virtual void print(AST *a = 0) const;
    virtual AST *assign(AST *a);
    virtual AST *Accept(ASTVisitor &);

  public:
    ModifierListType *_cnt_modifierList;
    ModifierListType *_cap_modifierList;
    ModifierListType *_lim_modifierList;
};

class DimensionType : public SignalTypeAST {
  public:
    DimensionType(ANTLRTokenPtr p, DimensionEntry *de);
    virtual astream        &operator<<(astream &s);
    virtual DimensionEntry *getDimension(AST *a = 0) const;
    virtual void            setDimension(AST *a = 0);

  private:
    DimensionEntry *m_dimensionEntry;
};

class DimensionedNumberType : public SignalTypeAST {
  public:
    DimensionedNumberType(ModifierEntry *me);
    virtual astream        &operator<<(astream &s);
    virtual AST            *data(AST *a = 0);
    virtual DimensionEntry *getDimension(AST *a = 0) const;
    virtual void            setDimension(AST *a = 0);

    virtual AST *eval(AST *a = 0);

    virtual Long   getInteger(int indx = 0) const;
    virtual double getDecimal(int indx = 0) const;
    virtual void   setInteger(int indx = 0, Long value = 0);
    virtual void   setDecimal(int indx = 0, double value = 0.0);

    virtual AST *min(AST *a = 0);
    virtual AST *max(AST *a = 0);

    virtual AST *check(AST *a = 0);

  protected:
    ModifierEntry *m_modifierEntry;
    AST           *m_val_ast;
};
class TimeQuantityType : public DimensionedNumberType {
  public:
    TimeQuantityType(ModifierEntry *me);
    virtual Long   getInteger(int indx = 0) const;
    virtual double getDecimal(int indx = 0) const;
};

class RangeType : public SignalTypeAST {
  public:
    RangeType(ANTLRTokenPtr p = 0);
    virtual AST *min(AST *a = 0);
    virtual AST *max(AST *a = 0);

  private:
};

class CnxNumeratorType : public SignalTypeAST {
  public:
    CnxNumeratorType(ANTLRTokenPtr p = 0);

  private:
};

class CnxDenominatorType : public SignalTypeAST {
  public:
    CnxDenominatorType(ANTLRTokenPtr p = 0);

  private:
};

class MeasuredCharacteristicMnemonicType : public ModifierType {
  public:
    MeasuredCharacteristicMnemonicType(ANTLRTokenPtr p, ModifierEntry *m, RWCString &left, class AtlasParser *parser = 0);
    virtual RWCString getName() const;
    virtual Long      compare(AST *o) const;
    virtual AST      *check(AST *a = 0);
    virtual AST      *data(AST *a = 0);
    virtual AST      *init(AST *a = 0);
    virtual void      print(AST *a = 0) const;
    virtual AST      *assign(AST *a);

    virtual AST *min(AST *a = 0);
    virtual AST *max(AST *a = 0);

  protected:
    // param-sfx : (-AV|-P|-PP|-P-NEG|-P-POS|-TRMS|-P-POS-ZERO|-P-NEG-ZERO) [-LMT]
    RWCString param_sfx;
    // ampl_sfx : param_sfx [-IN-PHASE|-QUAD]|-IN-PHASE|-QUAD|-INST
    RWCString ampl_sfx;
    // phase_sfx : -PHASE-(A|AB|AC|B|BC|CB|C|CA|BA)
    RWCString phase_sfx;
    // pulse_sfx : -P unsigned-integer-number
    RWCString pulse_sfx;
    RWCString ratio;
    enum Suffix { ParamSuffix,
                  AndAmplSuffix,
                  AmplSuffix,
                  PhaseSuffix,
                  RatioSuffix };
    void parse_suffixes(RWCString &text, Suffix suffix = ParamSuffix);
    int  ok(RWCString &text, const RWCString test, RWCString &dest);
};

class CharacteristicMnemonicType : public MeasuredCharacteristicMnemonicType {
  public:
    CharacteristicMnemonicType(ANTLRTokenPtr p, ModifierEntry *m, RWCString &left, class AtlasParser *parser = 0);

    virtual RWCString getName() const;
    virtual Long      compare(AST *o) const;
    virtual AST      *check(AST *a = 0);
    virtual AST      *data(AST *a = 0);
    virtual void      print(AST *a = 0) const;

  protected:
    RWCString cm_sfx; // -NUMERATOR , -DENOMINATOR , -QUIES , -TRANS , -REF
    void      parse_suffix(RWCString &text);
};

class TedlCharacteristicMnemonicType : public CharacteristicMnemonicType {
  public:
    TedlCharacteristicMnemonicType(ANTLRTokenPtr p, ModifierEntry *m, RWCString &left, class AtlasParser *parser = 0);

    virtual RWCString getName() const;
    // virtual Long		compare	( AST * o ) const;
    // virtual AST 	*	check	( AST * a=0 );
    // virtual	AST *	data	( AST * a = 0 );
    // virtual void	print	( AST * a = 0 ) const;
  protected:
    RWCString _sfx; // whatever is left...
};

class MeasuredCharacteristicType : public SignalTypeAST {
  public:
    MeasuredCharacteristicType(ANTLRTokenPtr p = 0);
    virtual RWCString getName() const;
    virtual TheType   getType(AST *a = 0) const;
    virtual AST      *data(AST *a = 0);
    virtual AST      *assign(AST *a);

  private:
};

class SignalValueType : public SignalTypeAST {
  public:
    SignalValueType(ANTLRTokenPtr p = 0);
    virtual AST *data(AST *a = 0);
    virtual void print(AST *a = 0) const;
    virtual AST *check(AST *a = 0);

  private:
};

class EventSlopePosType : public SignalTypeAST {
  public:
    EventSlopePosType(ANTLRTokenPtr p = 0);
    virtual RWCString getName() const;
    virtual AST      *data(AST *a = 0);
    virtual void      print(AST *a = 0) const;

  private:
};

class EventSlopeNegType : public SignalTypeAST {
  public:
    EventSlopeNegType(ANTLRTokenPtr p = 0);
    virtual RWCString getName() const;
    virtual AST      *data(AST *a = 0);
    virtual void      print(AST *a = 0) const;

  private:
};

class HysteresisType : public SignalTypeAST {
  public:
    HysteresisType(ANTLRTokenPtr p = 0);
    virtual AST *data(AST *a = 0);
    virtual void print(AST *a = 0) const;

  private:
};

#endif // SignalTypeAST_h
