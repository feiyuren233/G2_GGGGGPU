#include "codegen_instruction.h"

#include <cassert>
#include <string>
#include <sstream>
#include <iomanip>
#include <unordered_map>

#include "ast.h"
#include "common.h"
#include "parser.tab.h"

#define OP_FIELDWIDTH 12
#define REG_FIELDWIDTH 12
#define VALUE_FIELDWIDTH 12
#define SYMBOL_FIELDWIDTH 3

namespace COGEN
{
RegType::RegType(int dataType)
{
    switch (dataType)
    {
    case INT_T:
        m_value = RegType::INT;
        break;
    case FLOAT_T:
        m_value = RegType::FLOAT;
        break;
    case BOOL_T:
        m_value = RegType::INT;
        break;
    default:
        assert(0);
    }
}

std::string RegType::toString() const
{
    switch (m_value)
    {
    case RegType::INT:
        return "i";
    case RegType::FLOAT:
        return "f";
    case RegType::GENERAL:
        assert(0);
    }

    return "r";
}

std::string Register::toString() const
{
    std::string registerName = regType.toString();
    registerName += std::to_string(id);
    return registerName;
}

std::string MeowComment::generateCode() const
{
    if (m_comment == "")
    {
        return "";
    }
    else
    {
        return "# " + m_comment;
    }
}

std::string RInst::generateCode() const
{
    std::stringstream ss;

    assert(s_fncode.count(m_fnCode) == 1);

    ss << std::left << std::setw(OP_FIELDWIDTH) << s_fncode.at(m_fnCode);
    ss << std::left << std::setw(REG_FIELDWIDTH) << m_rd.toString();
    ss << std::left << std::setw(SYMBOL_FIELDWIDTH) << ",";
    ss << std::left << std::setw(REG_FIELDWIDTH) << m_ra.toString();
    ss << std::left << std::setw(SYMBOL_FIELDWIDTH) << ",";
    ss << std::left << std::setw(REG_FIELDWIDTH) << m_rb.toString();

    return ss.str();
}

std::string RFInst::generateCode() const
{
    std::stringstream ss;

    assert(s_fncode.count(m_fnCode) == 1);

    ss << std::left << std::setw(OP_FIELDWIDTH) << s_fncode.at(m_fnCode);
    ss << std::left << std::setw(REG_FIELDWIDTH) << m_rd.toString();
    ss << std::left << std::setw(SYMBOL_FIELDWIDTH) << ",";
    ss << std::left << std::setw(REG_FIELDWIDTH) << m_ra.toString();
    ss << std::left << std::setw(SYMBOL_FIELDWIDTH) << ",";
    ss << std::left << std::setw(REG_FIELDWIDTH) << m_rb.toString();
    ss << std::left << std::setw(SYMBOL_FIELDWIDTH) << ",";
    ss << std::left << std::setw(REG_FIELDWIDTH) << m_rc.toString();

    return ss.str();
}

std::string IInst::generateCode() const
{
    std::stringstream ss;

    assert(s_fncode.count(m_fnCode) == 1);

    ss << std::left << std::setw(OP_FIELDWIDTH) << s_fncode.at(m_fnCode);
    ss << std::left << std::setw(REG_FIELDWIDTH) << m_rd.toString();
    ss << std::left << std::setw(SYMBOL_FIELDWIDTH) << ",";
    ss << std::left << std::setw(REG_FIELDWIDTH) << m_ra.toString();
    ss << std::left << std::setw(SYMBOL_FIELDWIDTH) << ",";
    ss << std::left << std::setw(VALUE_FIELDWIDTH) << std::string("$") + m_imm12;

    return ss.str();
}

std::string SInst::generateCode() const
{
    std::stringstream ss;

    assert(s_fncode.count(m_fnCode) == 1);

    ss << std::left << std::setw(OP_FIELDWIDTH) << s_fncode.at(m_fnCode);
    ss << std::left << std::setw(REG_FIELDWIDTH) << m_ra.toString();
    ss << std::left << std::setw(SYMBOL_FIELDWIDTH) << ",";
    ss << std::left << std::setw(REG_FIELDWIDTH) << m_rb.toString();
    ss << std::left << std::setw(SYMBOL_FIELDWIDTH) << ",";
    ss << std::left << std::setw(VALUE_FIELDWIDTH) << std::string("$") + m_imm12;

    return ss.str();
}

std::string SBInst::generateCode() const
{
    std::stringstream ss;

    assert(s_fncode.count(m_fnCode) == 1);

    ss << std::left << std::setw(OP_FIELDWIDTH) << s_fncode.at(m_fnCode);
    ss << std::left << std::setw(REG_FIELDWIDTH) << m_ra.toString();
    ss << std::left << std::setw(SYMBOL_FIELDWIDTH) << ",";
    ss << std::left << std::setw(REG_FIELDWIDTH) << m_rb.toString();
    ss << std::left << std::setw(SYMBOL_FIELDWIDTH) << ",";
    ss << std::left << std::setw(VALUE_FIELDWIDTH) << std::string("$") + m_imm12;

    return ss.str();
}

std::string UInst::generateCode() const
{
    std::stringstream ss;

    assert(s_fncode.count(m_fnCode) == 1);

    ss << std::left << std::setw(OP_FIELDWIDTH) << s_fncode.at(m_fnCode);
    ss << std::left << std::setw(REG_FIELDWIDTH) << m_rd.toString();
    ss << std::left << std::setw(SYMBOL_FIELDWIDTH) << ",";
    ss << std::left << std::setw(VALUE_FIELDWIDTH) << std::string("$") + m_imm20;

    return ss.str();
}

std::string UJInst::generateCode() const
{
    std::stringstream ss;

    assert(s_fncode.count(m_fnCode) == 1);

    ss << std::left << std::setw(OP_FIELDWIDTH) << s_fncode.at(m_fnCode);
    ss << std::left << std::setw(REG_FIELDWIDTH) << m_rd.toString();
    ss << std::left << std::setw(SYMBOL_FIELDWIDTH) << ",";
    ss << std::left << std::setw(VALUE_FIELDWIDTH) << std::string("$") + m_imm20;

    return ss.str();
}

const std::unordered_map<RInst::FnCode, const char *> RInst::s_fncode = {
    {FnCode::OP_OR, "or"},
    {FnCode::OP_AND, "and"},
    {FnCode::OP_XOR, "xor"},
    {FnCode::OP_ADD, "add"},
    {FnCode::OP_SUB, "sub"},
    {FnCode::OP_MUL, "mul"},
    {FnCode::OP_CMPEQ_I, "cmpeq_i"},
    {FnCode::OP_CMPNE_I, "cmpne_i"},
    {FnCode::OP_CMPGT_I, "cmpgt_i"},
    {FnCode::OP_CMPGE_I, "cmpge_i"},
    {FnCode::OP_CMPLT_I, "cmplt_i"},
    {FnCode::OP_CMPLE_I, "cmple_i"},
    {FnCode::OP_CMPGT_U, "cmpgt_u"},
    {FnCode::OP_CMPGE_U, "cmpge_u"},
    {FnCode::OP_CMPLT_U, "cmplt_u"},
    {FnCode::OP_CMPLE_U, "cmple_u"},
    {FnCode::OP_ADD_F, "add_f"},
    {FnCode::OP_SUB_F, "sub_f"},
    {FnCode::OP_MUL_F, "mul_f"},
    {FnCode::OP_DIV_F, "div_f"},
    {FnCode::OP_CMPEQ_F, "cmpeq_f"},
    {FnCode::OP_CMPNE_F, "cmpne_f"},
    {FnCode::OP_CMPGT_F, "cmpgt_f"},
    {FnCode::OP_CMPGE_F, "cmpge_f"},
    {FnCode::OP_CMPLT_F, "cmplt_f"},
    {FnCode::OP_CMPLE_F, "cmple_f"}};

const std::unordered_map<RFInst::FnCode, const char *> RFInst::s_fncode = {
    {FnCode::OP_FMA_ADD, "fma_add"},
    {FnCode::OP_FMA_SUB, "fma_sub"}};

const std::unordered_map<IInst::FnCode, const char *> IInst::s_fncode = {
    {FnCode::OP_ASHR, "ashr"},
    {FnCode::OP_SHR, "shr"},
    {FnCode::OP_SHL, "shl"},
    {FnCode::OP_CLZ, "clz"},
    {FnCode::OP_CTZ, "ctz"},
    {FnCode::OP_ADDI, "addi"},
    {FnCode::OP_SUBI, "subi"},
    {FnCode::OP_ADDIU, "addiu"},
    {FnCode::OP_SUBIU, "subiu"},
    {FnCode::OP_MULI, "muli"},
    {FnCode::OP_RECIPROCAL, "rcp"},
    {FnCode::OP_ITOF, "itof"},
    {FnCode::OP_FTOI, "ftoi"},
    {FnCode::MEMOP_LOAD_I, "ld_i"},
    {FnCode::MEMOP_LL_I, "ll_i"},
    {FnCode::MEMOP_LOAD_F, "ld_f"},
    {FnCode::MEMOP_LL_F, "ll_f"},
    {FnCode::SYS_BLOCK_DIM, "blkdim"},
    {FnCode::SYS_BLOCK_IDX, "blkidx"},
    {FnCode::SYS_THREAD_IDX, "tidx"}};

const std::unordered_map<SInst::FnCode, const char *> SInst::s_fncode = {
    {FnCode::MEMOP_STORE_I, "st_i"},
    {FnCode::MEMOP_SC_I, "sc_i"},
    {FnCode::MEMOP_STORE_F, "st_f"},
    {FnCode::MEMOP_SC_F, "sc_f"}};

const std::unordered_map<SBInst::FnCode, const char *> SBInst::s_fncode = {
    {FnCode::IF, "if"},
    {FnCode::ELSE, "else"},
    {FnCode::ELIF, "elif"},
    {FnCode::ENDIF, "endif"},
    {FnCode::EXIT, "exit"}};

const std::unordered_map<UInst::FnCode, const char *> UInst::s_fncode = {
    {FnCode::OP_LUI_I, "lui_i"},
    {FnCode::OP_LLI_I, "lli_i"},
    {FnCode::OP_LUI_F, "lui_f"},
    {FnCode::OP_LLI_F, "lli_f"}};

const std::unordered_map<UJInst::FnCode, const char *> UJInst::s_fncode = {
    {FnCode::OP_JMP, "jmp"}};

} // namespace COGEN