#ifndef CODEGEN_INSTRUCTION_H_INCLUDED
#define CODEGEN_INSTRUCTION_H_INCLUDED

#include <string>
#include <unordered_map>

namespace COGEN
{

class RegType
{
  public:
    enum Value
    {
        INT,
        FLOAT,
        GENERAL
    };

  public:
    RegType() = default;
    RegType(const RegType &regType) : m_value(regType.m_value) {}
    constexpr explicit RegType(Value regType) : m_value(regType) {}
    explicit RegType(int dataType);

    RegType &operator=(const RegType &other)
    {
        m_value = other.m_value;
        return *this;
    }
    RegType &operator=(Value other)
    {
        m_value = other;
        return *this;
    }

    bool operator==(RegType other) const { return m_value == other.m_value; }
    bool operator!=(RegType other) const { return m_value != other.m_value; }

    bool operator==(Value other) const { return m_value == other; }
    bool operator!=(Value other) const { return m_value != other; }

  public:
    std::string toString() const;
    Value getValue() const { return m_value; }

  private:
    Value m_value;
};

struct Register
{
    RegType regType = RegType(RegType::GENERAL);
    unsigned id = 0;

    std::string toString() const;
};

class Instruction
{
  public:
    virtual std::string generateCode() const = 0;

  public:
    virtual ~Instruction() = default;
};

class MeowComment : public Instruction
{
  private:
    std::string m_comment;

  public:
    MeowComment(const std::string &comment) : m_comment(comment) {}
    MeowComment(std::string &&comment) : m_comment(std::move(comment)) {}

    virtual ~MeowComment() = default;

  public:
    virtual std::string generateCode() const;
};

class RInst : public Instruction
{
  public:
    enum class FnCode
    {
        // Integer pipeline
        OP_OR,      // Bitwise logical or
        OP_AND,     // bitwise logical and
        OP_XOR,     // bitwise logical exclusive or
        OP_ADD,     // Add integer
        OP_SUB,     // Subtract integer
        OP_MUL,     // Multiply integer low
        OP_CMPEQ_I, // Integer equal
        OP_CMPNE_I, // Integer not equal
        OP_CMPGT_I, // Integer greater (signed)
        OP_CMPGE_I, // Integer greater or equal (signed)
        OP_CMPLT_I, // Integer less than (signed)
        OP_CMPLE_I, // Integer less than or equal (signed)
        OP_CMPGT_U, // Integer greater than (unsigned)
        OP_CMPGE_U, // Integer greater or equal (unsigned)
        OP_CMPLT_U, // Integer less than (unsigned)
        OP_CMPLE_U, // Integer less than or equal (unsigned)

        // FP pineline
        OP_ADD_F, // Floating point add
        OP_SUB_F, // Floating point substract
        OP_MUL_F, // Floating point multiply
        OP_DIV_F, // Floating point divide

        OP_CMPGT_F, // Floating point greater than
        OP_CMPLT_F, // Floating point less than
        OP_CMPGE_F, // Floating point greater or equal
        OP_CMPLE_F, // Floating point less than or equal
        OP_CMPEQ_F, // Floating point equal
        OP_CMPNE_F  // Floating point not-equal
    };

  public:
    static const std::unordered_map<FnCode, const char *> s_fncode;

  public:
    RInst(FnCode fnCode, Register rd, Register ra, Register rb) : m_fnCode(fnCode), m_ra(ra), m_rb(rb), m_rd(rd) {}
    virtual ~RInst() = default;

  public:
    virtual std::string generateCode() const;

  private:
    FnCode m_fnCode;
    Register m_ra;
    Register m_rb;
    Register m_rd;
};

class RFInst : public Instruction
{
  public:
    enum class FnCode
    {
        // FP pipeline
        OP_FMA_ADD, // FMA a * b + c
        OP_FMA_SUB  // FMA a * b - c
    };

  public:
    static const std::unordered_map<FnCode, const char *> s_fncode;

  public:
    RFInst(FnCode fnCode, Register rd, Register ra, Register rb, Register rc) : m_fnCode(fnCode), m_ra(ra), m_rb(rb), m_rc(rc), m_rd(rd) {}
    virtual ~RFInst() = default;

  public:
    virtual std::string generateCode() const;

  private:
    FnCode m_fnCode;
    Register m_ra;
    Register m_rb;
    Register m_rc;
    Register m_rd;
};

class IInst : public Instruction
{
  public:
    enum class FnCode
    {
        // Integer pipeline
        OP_ASHR, // Arithmetic shift right (sign extend)
        OP_SHR,  // Logical shift right (no sign extend)
        OP_SHL,  // Logical shift left
        OP_CLZ,  // Count leading zeroes
        OP_CTZ,  // Count trailing zeroes
        OP_ADDI,
        OP_SUBI,
        OP_ADDIU,
        OP_SUBIU,
        OP_MULI,

        // WARNING! NOT IMPLEMENTED
        OP_RECIPROCAL, // Reciprocal estimate

        // Mixed pipeline
        OP_ITOF, // Integer to float
        OP_FTOI, // Float to integer

        // Mem pipeline
        MEMOP_LOAD_I, // Regular load integer
        MEMOP_LL_I,   // Load linked integer
        MEMOP_LOAD_F, // Regular load float
        MEMOP_LL_F,   // Load linked float

        // System functions
        SYS_BLOCK_DIM,  // blockDim
        SYS_BLOCK_IDX,  // blockIdx
        SYS_THREAD_IDX, // threadIdx
    };

  public:
    static const std::unordered_map<FnCode, const char *> s_fncode;

  public:
    IInst(FnCode fnCode, Register rd, Register ra, std::string imm12) : m_fnCode(fnCode), m_ra(ra), m_rd(rd), m_imm12(imm12) {}
    virtual ~IInst() = default;

  public:
    virtual std::string generateCode() const;

  private:
    FnCode m_fnCode;
    Register m_ra;
    Register m_rd;
    std::string m_imm12;
};

class SInst : public Instruction
{
  public:
    enum class FnCode
    {
        // Mem pipeline
        MEMOP_STORE_I, // Regular store int
        MEMOP_SC_I,    // Store cond. int
        MEMOP_STORE_F, // Regular store float
        MEMOP_SC_F     // Store cond. float
    };

  public:
    static const std::unordered_map<FnCode, const char *> s_fncode;

  public:
    SInst(FnCode fnCode, Register ra, Register rb, std::string imm12) : m_fnCode(fnCode), m_ra(ra), m_rb(rb), m_imm12(imm12) {}
    virtual ~SInst() = default;

  public:
    virtual std::string generateCode() const;

  private:
    FnCode m_fnCode;
    Register m_ra;
    Register m_rb;
    std::string m_imm12;
};

class SBInst : public Instruction
{
  public:
    enum class FnCode
    {
        IF,    // If
        ELSE,  // Else if
        ELIF,  // Else
        ENDIF, // Endif
        EXIT   // Exit
    };

  public:
    static const std::unordered_map<FnCode, const char *> s_fncode;

  public:
    SBInst(FnCode fnCode, Register ra, Register rb, std::string imm12) : m_fnCode(fnCode), m_ra(ra), m_rb(rb), m_imm12(imm12) {}
    virtual ~SBInst() = default;

  public:
    virtual std::string generateCode() const;

  private:
    FnCode m_fnCode;
    Register m_ra;
    Register m_rb;
    std::string m_imm12;
};

class UInst : public Instruction
{
  public:
    enum class FnCode
    {
        OP_LUI_I, // Load upper immediate 20 I
        OP_LLI_I, // Load lower immediate 20 I
        OP_LUI_F, // Load upper immediate 20 F
        OP_LLI_F, // Load lower immediate 20 F
    };

  public:
    static const std::unordered_map<FnCode, const char *> s_fncode;

  public:
    UInst(FnCode fnCode, Register rd, std::string imm20) : m_fnCode(fnCode), m_rd(rd), m_imm20(imm20) {}
    virtual ~UInst() = default;

  public:
    virtual std::string generateCode() const;

  private:
    FnCode m_fnCode;
    Register m_rd;
    std::string m_imm20;
};

class UJInst : public Instruction
{
  public:
    enum class FnCode
    {
        OP_JMP // Load upper immediate 20
    };

  public:
    static const std::unordered_map<FnCode, const char *> s_fncode;

  public:
    UJInst(FnCode fnCode, Register rd, std::string imm20) : m_fnCode(fnCode), m_rd(rd), m_imm20(imm20) {}
    virtual ~UJInst() = default;

  public:
    virtual std::string generateCode() const;

  private:
    FnCode m_fnCode;
    Register m_rd;
    std::string m_imm20;
};

} // namespace COGEN

#endif