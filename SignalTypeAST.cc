
#include "AtlasStd.h"
#include "SignalTypeAST.h"
#include "AtlasParser.h"
#include "Visitors.h"

SignalTypeAST::SignalTypeAST(ANTLRTokenPtr p, TheType t) : AST(p), _storage(t) {
}
SignalTypeAST::~SignalTypeAST() {
}

AST *SignalTypeAST::eval(AST *a) {
    return ASTdown()->eval(a);
}
AST *SignalTypeAST::assign(AST *a) {
    return this;
}
AST *SignalTypeAST::check(AST *a) {
    return ASTdown()->check(a);
}
Long SignalTypeAST::compare(AST *o) const {
    return ASTdown()->compare(o);
}
TheType SignalTypeAST::getType(AST *a) const {
    return ASTdown()->getType(a);
}
AST *SignalTypeAST::Accept(ASTVisitor &v) {
    return v.VisitSignalTypeAST(this);
}
AST *SignalTypeAST::data(AST *a) {
    return ASTdown()->data(a);
}

AnalogType ::AnalogType(ANTLRTokenPtr p) : SignalTypeAST(p) {};
DoSimulStructureType ::DoSimulStructureType(ANTLRTokenPtr p) : SignalTypeAST(p) {};
DoSimulStatementType ::DoSimulStatementType(ANTLRTokenPtr p) : SignalTypeAST(p) {};
DoSimulBodyType ::DoSimulBodyType(ANTLRTokenPtr p) : SignalTypeAST(p) {};
DoSimulEndDoType ::DoSimulEndDoType(ANTLRTokenPtr p) : SignalTypeAST(p) {};

NounType::NounType(ANTLRTokenPtr p, NounEntry *ne)
    : SignalTypeAST(p), _nounEntry(ne) {};

AST *NounType::eval(AST *a) {
    if (a != 0)
        return ASTdown()->eval(a);
    else
        return this;
}

AST *NounType::check(AST *a) {
    if ((a) && compare(a) == 0) {
        if (ASTdown()) {
            if (ASTdown()->check(a->ASTdown())) {
                return this;
            }
        } else {
            return this;
        }
    }
    return 0;
}

Long NounType::compare(AST *o) const {
    RWCString other = o->getName();

    if (getName() < other) {
        return -1;
    } else {
        return (getName() > other);
    }
};

RWCString NounType::getName() const {
    return _nounEntry->noun;
}
AST *NounType::data(AST *a) {
    if (!a) {
        return this;
    } else {
        AST *what = ASTdown()->data(a);
        if (what == 0) {
            AST *where = ASTdown()->ASTright();
            if (where) {
                return where->data(a);
            }
        }
        return what;
    }
}

AST *NounType::init(AST *a) {
    if (a) {
        return ASTdown()->init(a);
    } else {
        ASTdown()->init(this);
    }
    return this;
}

astream &
NounType::operator<<(astream &s) {
    if (ASTdown()) {
        s << "(";
        s << ASTdown();
        s << ")";
    }
    s << ",";
    s << _nounEntry->noun;
    s << " ";
    return s;
}

StatementCharacteristics::StatementCharacteristics(NounEntry *ne)
    : _nounEntry(ne) {};

StatementCharacteristic::StatementCharacteristic(ModifierEntry *me) {};

Long StatementCharacteristic::compare(AST *o) const // o is the superset
{
    assert(0);
    return 0;
}

RWCString StatementCharacteristic::getName() const {
    return ASTdown()->getName();
}

AST *StatementCharacteristic::check(AST *a) {
    AST *checking;
    checking = ASTdown();
    if (!checking->check(a)) {
        return 0;
    }
    checking = checking->ASTright();
    if (!checking->check(a)) {
        return 0;
    }
    checking = checking->ASTright();
    if (!checking) {
        return this;
    }
    if (!checking->check(a)) {
        return 0;
    }
    return this;
}

AST *StatementCharacteristic::eval(AST *a) {
    return ASTdown()->ASTright()->eval(a);
}

TheType StatementCharacteristic::getType(AST *a) const {
    return ASTdown()->ASTright()->getType(a);
}

ModifierType::ModifierType(ANTLRTokenPtr p, ModifierEntry *me, AtlasParser *parser)
    : SignalTypeAST(p), m_modifierEntry(me), m_parser(parser) {
}

RWCString ModifierType::getName() const {
    return m_modifierEntry->modifier;
}

Long ModifierType::compare(AST *o) const // o is the superset
{

    StringAST modifier(getName());
    AST      *what = o->data(&modifier);

    RWCString other = what->getName();

    if (getName() < other) {
        return -1;
    } else {
        return (getName() > other);
    }
}

AST *ModifierType::check(AST *a) {
    if ((a) && compare(a) == 0) {
        return this;
    }
    return 0;
}

AST *ModifierType::data(AST *a) {
    if (!a) {
        return this;
    } else if (a->getName() == getName()) {
        return this;
    } else {
        return 0;
    }
}

void ModifierType::print(AST *a) const {
    std::cout << "Modifier Print>>" << getName() << std::endl;
}

AST *ModifierType::init(AST *a) {
    RWCString      left;
    ModifierEntry *modifierEntry;
    NounEntry     *nounEntry = ((NounType *)a)->_nounEntry;
    modifierEntry            = m_parser->theModifierEntry(getName(), nounEntry, left);
    if (!modifierEntry) {
        Error_Report("NOUN " + nounEntry->noun + " Does not support " + m_modifierEntry->modifier, getToken());
    } else if (modifierEntry != m_modifierEntry) {
        std::cout << "Changed modifier " << std::endl;
        m_modifierEntry = modifierEntry;
    }
    return this;
}

ModifierListType::ModifierListType(ASTList *l)
    : _modifierList(l) {};

AST *ModifierListType::data(AST *a) {
    AST *mod = 0;
    _modifierList->findValue(a->getName(), mod);
    return mod;
}

AST *ModifierListType::check(AST *a) // we are the subset,a is the superset
{
    AST *subset = this,
        *supset = a;
    AST *result = this;
    AST *mod    = 0;
    // ASTListIterator		it( *_modifierList );

    // for each MODIFIER in the _modifierList,
    // make sure we are covered.

    ////		while ( ++it ){
    ////			StringAST modifier(it.key()->getName());
    ////			if(mod=supset->data(&modifier)){
    ////				if(  it.key()->check( mod ) ){
    ////					result = this;
    ////				} else {
    ////					return 0;
    ////				}
    ////			} else {	// oops , modifier not found!!!
    ////				//Error_Report(" Modifier is not covered. ", it.key());
    ////				return 0;
    ////			}
    ////
    ////		}

    for (const auto &astPtr : *_modifierList) {
        StringAST modifier(astPtr->getName());

        if (AST *mod = supset->data(&modifier)) {
            if (astPtr->check(mod)) {
                result = this;
            } else {
                return nullptr;
            }
        } else {
            return nullptr;
        }
    }

    return result;
}
AST *ModifierListType::assign(AST *a) {
    _modifierList->insert(a);
    return this;
}

AST *ModifierListType::Accept(ASTVisitor &v) {
    return v.VisitModifierListType(this);
}

Long ModifierListType::compare(AST *o) const // we are the superset
{
    AST *subset = o;
    // ASTListIterator		it( *_modifierList );

    long result = -2;
    for (const auto &it : *_modifierList) {
        assert(0);

        switch (subset->compare(it)) {
            case 1:
                result = 1;
                break;

            case 0: // equal modifiers
                if (result == -2) {
                    result = 0;
                };
                break;
            case -1: // superset greater than subset
                if (result == -2) {
                    result = -1;
                };
            default:
                result = 0;
        }
    }
    return result;
}

void ModifierListType::print(AST *a) const {

    // ASTListIterator		it( *_modifierList );

    // for each MODIFIER in the _modifierList,

    for (const auto &it : *_modifierList) {
        it->print();
    }
}

astream &
ModifierListType::operator<<(astream &s) {
    // ASTListIterator		it( *_modifierList );

    // for each MODIFIER in the _modifierList,

    int printed = 0;

    for (const auto &it : *_modifierList) {
        if (printed) {
            s << ",";
        }
        s << it->getName();
        printed = 1;
    }
    return s;
}

AST *ModifierListType::init(AST *a) {

    // ASTListIterator		it( *_modifierList );

    // for each MODIFIER in the _modifierList,

    for (const auto &it : *_modifierList) {
        it->init(a);
    }
    return this;
}

ReqModifierListType::ReqModifierListType(ASTList *cntl, ASTList *capl, ASTList *liml)
    : ModifierListType(new ASTList) {
    _cnt_modifierList = new ModifierListType(cntl); // Control
    _cap_modifierList = new ModifierListType(capl); // Capability
    _lim_modifierList = new ModifierListType(liml); // Limit

    for (const auto &it : *_cnt_modifierList) {
        _modifierList->append(it);
    }
    for (const auto &it : *_cap_modifierList) {
        _modifierList->append(it);
    }
    for (const auto &it : *_lim_modifierList) {
        _modifierList->append(it);
    }
}

AST *ReqModifierListType::data(AST *a) {
    AST *mod = 0;

    if (mod = _cnt_modifierList->data(a)) {
        return mod;
    } else if (mod = _cap_modifierList->data(a)) {
        return mod;
    }
    return _lim_modifierList->data(a);
}

AST *ReqModifierListType::check(AST *a) // we are the subset,a is the superset
{
    AST *cnt = _cnt_modifierList->check(a);
    AST *cap = _cap_modifierList->check(a);
    AST *lim = _lim_modifierList->check(a);

    AST *result = this;
    if (cnt == NULL)
        result = NULL;
    if (cap == NULL)
        result = NULL;
    if (lim == NULL)
        result = NULL;

    return result;
}

AST *ReqModifierListType::assign(AST *a) {
    _modifierList->insert(a);
    return this;
}

AST *ReqModifierListType::Accept(ASTVisitor &v) {
    return v.VisitReqModifierListType(this);
}

Long ReqModifierListType::compare(AST *o) const // we are the superset
{

    int cnt = _cnt_modifierList->compare(o);
    int cap = _cap_modifierList->compare(o);
    int lim = _lim_modifierList->compare(o);

    int result = -2;
    result     = (result > cnt) ? result : cnt;
    result     = (result > cap) ? result : cap;
    result     = (result > lim) ? result : lim;

    return result;
}

void ReqModifierListType::print(AST *a) const {
    _cnt_modifierList->print(a);
    _cap_modifierList->print(a);
    _lim_modifierList->print(a);
}

AST *ReqModifierListType::init(AST *a) {
    _cnt_modifierList->init(a);
    _cap_modifierList->init(a);
    _lim_modifierList->init(a);
    return this;
}

MeasuredCharacteristicsType::MeasuredCharacteristicsType(ASTList *l)
    : ModifierListType(l) {};

DimensionType::DimensionType(ANTLRTokenPtr p, DimensionEntry *de)
    : m_dimensionEntry(de) {};

DimensionEntry *
DimensionType::getDimension(AST *a) const {
    return m_dimensionEntry;
}

void DimensionType::setDimension(AST *a) {
    DimensionEntry *d = a ? a->getDimension() : 0;
    if (d)
        m_dimensionEntry = d;
}

astream &
DimensionType::operator<<(astream &s) {
    s << m_dimensionEntry->dimension;
    return s;
}

DimensionedNumberType::DimensionedNumberType(ModifierEntry *me)
    : m_modifierEntry(me), m_val_ast(new DecimalNumber) {
}

astream &
DimensionedNumberType::operator<<(astream &s) {
    AST *n = ASTdown()->eval();
    AST *d = ASTdown()->ASTright();
    // Attempted to print out
    // LL 'LO'=1
    // when limit is a variable.
    // And promptly got in trouble with
    // NegativeOperator..
    // Instead it came out
    // LL -=-1
    // so disabled it for now....

    // if(n==ASTdown()){
    s << n;
    //} else {
    //	s << ASTdown();
    //	s << "=";
    //	s << n;
    //}

    if (d) {
        s << " ";
        s << d;
    }
    s << " ";
    return s;
}

AST *DimensionedNumberType::data(AST *a) {
    return this;
}

DimensionEntry *
DimensionedNumberType::getDimension(AST *a) const {
    return ASTdown()->ASTright()->getDimension();
}

void DimensionedNumberType::setDimension(AST *a) {
    ASTdown()->ASTright()->setDimension(a);
}

Long DimensionedNumberType::getInteger(int indx) const {
    return getDecimal();
}

double
DimensionedNumberType::getDecimal(int indx) const {
    return ScaleDimension(ASTdown()->getDecimal(), getDimension());
}

void DimensionedNumberType::setInteger(int indx, Long value) {
    setDecimal(value);
}

void DimensionedNumberType::setDecimal(int indx, double value) {
    ASTdown()->setDecimal(0, ScaleDimension(value, 0, getDimension()));
}

AST *DimensionedNumberType::eval(AST *a) {
    ASTdown()->eval(a);
    m_val_ast->setDecimal(0, getDecimal());
    return m_val_ast;
}

AST *DimensionedNumberType::min(AST *a) {
    eval();
    return m_val_ast;
}

AST *DimensionedNumberType::max(AST *a) {
    eval();
    return m_val_ast;
}

AST *DimensionedNumberType::check(AST *a) {
    if (a) {
        AST *loc;
        // a->check(this); see if ByQuantization covers us....
        loc = a->max(this);
        if ((loc != NULL) && compare(loc) > 0) {
            return 0;
        }
        loc = a->min(this);
        if ((loc != NULL) && compare(loc) < 0) {
            return 0;
        }
        return this;
    } else {
        return SignalTypeAST::check(); // whatever....
    }
}

TimeQuantityType::TimeQuantityType(ModifierEntry *me)
    : DimensionedNumberType(me) {
}

Long TimeQuantityType::getInteger(int indx) const {
    return DimensionedNumberType::getDecimal();
}

double
TimeQuantityType::getDecimal(int indx) const {
    double x = DimensionedNumberType::getDecimal();
    if (x < 0) {
        Error_Report(" Negative Time is not allowed. 14.11.4");
        return 0;
    }
    return x;
}

RangeType ::RangeType(ANTLRTokenPtr p) {
}

AST *RangeType::min(AST *a) {
    return ASTdown()->min(a);
}

AST *RangeType::max(AST *a) {
    return ASTdown()->ASTright()->max(a);
}

CnxNumeratorType ::CnxNumeratorType(ANTLRTokenPtr p) {};
CnxDenominatorType ::CnxDenominatorType(ANTLRTokenPtr p) {};

MeasuredCharacteristicMnemonicType::MeasuredCharacteristicMnemonicType(ANTLRTokenPtr p, ModifierEntry *m, RWCString &left, AtlasParser *parser)
    : ModifierType(p, m, parser) {
    parse_suffixes(left);
}

RWCString MeasuredCharacteristicMnemonicType::getName() const {
    return ModifierType::getName() + ampl_sfx + phase_sfx + pulse_sfx + ratio;
}

Long MeasuredCharacteristicMnemonicType::compare(AST *o) const // o is the superset
{

    StringAST modifier(getName());
    AST      *what = o->data(&modifier);

    RWCString other = what->getName();

    if (getName() < other) {
        return -1;
    } else {
        return (getName() > other);
    }
}

AST *MeasuredCharacteristicMnemonicType::check(AST *a) {
    if ((a) && compare(a) == 0) {
        return this;
    }
    return 0;
}

AST *MeasuredCharacteristicMnemonicType::data(AST *a) {
    if (!a) {
        return this;
    } else if (a->getName() == getName()) {
        return this;
    } else {
        return 0;
    }
}

AST *MeasuredCharacteristicMnemonicType::init(AST *a) {
    ModifierType::init(a);
    if (!m_modifierEntry->isUsage("M")) {
        Error_Report("MODIFIER is not an M usage type:" + m_modifierEntry->modifier, this);
        return 0;
    }
    // now we got to make sure that the suffixes are covered..
    return this;
}

AST *MeasuredCharacteristicMnemonicType::assign(AST *a) {
    return ASTdown()->assign(a);
}

void MeasuredCharacteristicMnemonicType::print(AST *a) const {
    std::cout << " MeasuredCharacteristicMnemonicType >>" << getName() << std::endl;
}

int MeasuredCharacteristicMnemonicType::ok(RWCString &text, const RWCString test, RWCString &dest) {
    if (text.find(test) == 0) {
        dest = test;
        text.erase(0, test.length());
        return 1;
    } else {
        return 0;
    }
}

void MeasuredCharacteristicMnemonicType::parse_suffixes(RWCString &text, Suffix suffix) {
    if (text == "")
        return;
    switch (suffix) {
        case ParamSuffix: {
            if (ok(text, "-TRMS", param_sfx)) {
            } else if (ok(text, "-PP", param_sfx)) {
            } else if (ok(text, "-P-POS-ZERO", param_sfx)) {
            } else if (ok(text, "-P-POS", param_sfx)) {
            } else if (ok(text, "-P-NEG-ZERO", param_sfx)) {
            } else if (ok(text, "-P-NEG", param_sfx)) {
            } else if (ok(text, "-P", param_sfx)) {
            } else if (ok(text, "-AV", param_sfx)) {
            }
            if (param_sfx != "") {
                RWCString scratch_sfx;
                ok(text, "-LMT", scratch_sfx);
                param_sfx += scratch_sfx;
                parse_suffixes(text, AndAmplSuffix);
            } else {
                parse_suffixes(text, AmplSuffix);
            }
            break;
        }
        case AndAmplSuffix: {
            if (ok(text, "-IN-PHASE", ampl_sfx)) {
            } else if (ok(text, "-QUAD", ampl_sfx)) {
            }

            parse_suffixes(text, PhaseSuffix);
            break;
        }
        case AmplSuffix: {
            if (ok(text, "-IN-PHASE", ampl_sfx)) {
            } else if (ok(text, "-QUAD", ampl_sfx)) {
            } else if (ok(text, "-INST", ampl_sfx)) {
            }
            parse_suffixes(text, PhaseSuffix);
        }
        case PhaseSuffix: {
            if (ok(text, "-PHASE-AB", ampl_sfx)) {
            } else if (ok(text, "-PHASE-AC", ampl_sfx)) {
            } else if (ok(text, "-PHASE-BA", ampl_sfx)) {
            } else if (ok(text, "-PHASE-BC", ampl_sfx)) {
            } else if (ok(text, "-PHASE-CA", ampl_sfx)) {
            } else if (ok(text, "-PHASE-CB", ampl_sfx)) {
            } else if (ok(text, "-PHASE-A", ampl_sfx)) {
            } else if (ok(text, "-PHASE-B", ampl_sfx)) {
            } else if (ok(text, "-PHASE-C", ampl_sfx)) {
            }
            parse_suffixes(text, RatioSuffix);
        }
        case RatioSuffix: {
            ok(text, "-RATIO", ratio);
        }
    }
}

AST *MeasuredCharacteristicMnemonicType::min(AST *a) {
    return ASTdown()->min(a);
}

AST *MeasuredCharacteristicMnemonicType::max(AST *a) {
    return ASTdown()->max(a);
}

CharacteristicMnemonicType::CharacteristicMnemonicType(ANTLRTokenPtr p, ModifierEntry *m, RWCString &left, AtlasParser *parser)
    : MeasuredCharacteristicMnemonicType(p, m, left, parser) {
    parse_suffix(left);
}

RWCString CharacteristicMnemonicType::getName() const {
    return MeasuredCharacteristicMnemonicType::getName() + cm_sfx;
}

Long CharacteristicMnemonicType::compare(AST *o) const // o is the superset
{

    StringAST modifier(getName());
    AST      *what = o->data(&modifier);

    RWCString other = what->getName();

    if (getName() < other) {
        return -1;
    } else {
        return (getName() > other);
    }
}

AST *CharacteristicMnemonicType::check(AST *a) {
    if ((a) && compare(a) == 0) {
        if (ASTdown() && a->ASTdown()) {
            return ASTdown()->check(a->data()->ASTdown());
        }
        return this; // no range etc.. means accept all
    }
    return 0;
}

AST *CharacteristicMnemonicType::data(AST *a) {
    if (!a) {
        return this;
    } else if (a->getName() == getName()) {
        return this;
    } else {
        return 0;
    }
}

void CharacteristicMnemonicType::print(AST *a) const {
    std::cout << "CharacteristicMnemonicType Print>>" << getName() << std::endl;
}

void CharacteristicMnemonicType::parse_suffix(RWCString &text) {
    if (text == "")
        return;
    if (ok(text, "-NUMERATOR", cm_sfx)) {
    } else if (ok(text, "-DENOMINATOR", cm_sfx)) {
    } else if (ok(text, "-QUIES", cm_sfx)) {
    } else if (ok(text, "-TRANS", cm_sfx)) {
    } else if (ok(text, "-REF", cm_sfx)) {
    }
}

TedlCharacteristicMnemonicType::TedlCharacteristicMnemonicType(ANTLRTokenPtr p, ModifierEntry *m, RWCString &left, AtlasParser *parser)
    : CharacteristicMnemonicType(p, m, left, parser) {
    _sfx = left; // accept whatever is left
    left = "";
}

RWCString TedlCharacteristicMnemonicType::getName() const {
    return CharacteristicMnemonicType::getName() + _sfx;
}

MeasuredCharacteristicType::MeasuredCharacteristicType(ANTLRTokenPtr p) {
}

RWCString MeasuredCharacteristicType::getName() const {
    return ASTdown()->getName();
}

TheType MeasuredCharacteristicType::getType(AST *a) const {
    return ASTdown()->getType(a);
}

AST *MeasuredCharacteristicType::assign(AST *a) {
    AST *target = 0;
    if (target = ASTdown()) {
        target->assign(a);
    }
    return target;
}
AST *MeasuredCharacteristicType::data(AST *a) {
    return ASTdown()->data(a);
}

SignalValueType::SignalValueType(ANTLRTokenPtr p)
    : SignalTypeAST(p) {
}

AST *SignalValueType::data(AST *a) {
    return ASTdown()->eval();
    ;
}

AST *SignalValueType::check(AST *a) {
    // right now only check range... run-time
    // must check ASTdown() is within range...
    return ASTdown()->ASTright()->check(a);
}

void SignalValueType::print(AST *a) const {
    std::cout << " SignalValueType >>" << getName() << std::endl;
}

EventSlopePosType::EventSlopePosType(ANTLRTokenPtr p)
    : SignalTypeAST(p, EventSlopePosTypeValue) {
}

RWCString EventSlopePosType::getName() const {
    return SignalTypeAST::getName();
}

AST *EventSlopePosType::data(AST *a) {
    if (!a) {
        return this;
    } else if (a->getName() == getName()) {
        return this;
    } else if (ASTdown()) {
        return ASTdown()->data(a);
        ;
    } else {
        return 0;
    }
}

void EventSlopePosType::print(AST *a) const {
    std::cout << " EventSlopePosType >>" << getName() << std::endl;
}

EventSlopeNegType::EventSlopeNegType(ANTLRTokenPtr p)
    : SignalTypeAST(p, EventSlopeNegTypeValue) {
}

RWCString EventSlopeNegType::getName() const {
    return SignalTypeAST::getName();
}

AST *EventSlopeNegType::data(AST *a) {
    if (!a) {
        return this;
    } else if (a->getName() == getName()) {
        return this;
    } else if (ASTdown()) {
        return ASTdown()->data(a);
        ;
    } else {
        return 0;
    }
}

void EventSlopeNegType::print(AST *a) const {
    std::cout << " EventSlopeNegType >>" << getName() << std::endl;
}
HysteresisType::HysteresisType(ANTLRTokenPtr p)
    : SignalTypeAST(p, HysteresisTypeValue) {
}

AST *HysteresisType::data(AST *a) {
    return ASTdown()->eval();
    ;
}

void HysteresisType::print(AST *a) const {
    std::cout << " HysteresisType >>" << getName() << std::endl;
}
