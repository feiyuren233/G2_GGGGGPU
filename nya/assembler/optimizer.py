from optimization_cfg import *
from optimization_liveness import *
from optimization_regalloc import *
import copy


def optimizer(instructions):
    # [(fncode, (operands))]

    # Construct CFG
    cfg = construct_cfg(instructions)

    # Construct Live Set
    global_liveset = construct_liveset(cfg, instructions)

    # Visualize CFG
    visualize_cfg(cfg, instructions, global_liveset)

    # Register Allocation
    r = allocate_register(instructions, global_liveset)

    # Remap registers in the original instructions to optimized registers allocation
    if r:
        i_logical_to_physical_mapping, f_logical_to_physical_mapping = r
        register_allocation = i_logical_to_physical_mapping.copy()
        register_allocation.update(f_logical_to_physical_mapping)

        return remap_registers_in_instructions(instructions, register_allocation)
    else:
        return None


def remap_registers_in_instructions(instructions, register_allocation):
    # [(fncode, (operands))]
    # logical_to_physical_mapping = reg name -> physical reg name

    new_instructions = []

    for instruction in instructions:
        fncode, operands = instruction

        new_operands = ()
        if isinstance(fncode, RtypeFnCode):
            rd, ra, rb = operands
            rd = register_allocation.get(rd, "r0")
            ra = register_allocation.get(ra, "r0")
            rb = register_allocation.get(rb, "r0")
            new_operands = (rd, ra, rb)
        elif isinstance(fncode, RFtypeFnCode):
            rd, ra, rb, rc = operands
            rd = register_allocation.get(rd, "r0")
            ra = register_allocation.get(ra, "r0")
            rb = register_allocation.get(rb, "r0")
            rc = register_allocation.get(rc, "r0")
            new_operands = (rd, ra, rb, rc)
        elif isinstance(fncode, ItypeFnCode):
            rd, ra, imm = operands
            rd = register_allocation.get(rd, "r0")
            ra = register_allocation.get(ra, "r0")
            new_operands = (rd, ra, imm)
        elif isinstance(fncode, StypeFnCode):
            ra, rb, imm = operands
            ra = register_allocation.get(ra, "r0")
            rb = register_allocation.get(rb, "r0")
            new_operands = (ra, rb, imm)
        elif isinstance(fncode, SBtypeFnCode):
            ra, rb, imm = operands
            ra = register_allocation.get(ra, "r0")
            rb = register_allocation.get(rb, "r0")
            new_operands = (ra, rb, imm)
        elif isinstance(fncode, UtypeFnCode):
            rd, imm = operands
            rd = register_allocation.get(rd, "r0")
            new_operands = (rd, imm)
        elif isinstance(fncode, UJtypeFnCode):
            rd, imm = operands
            rd = register_allocation.get(rd, "r0")
            new_operands = (rd, imm)
        else:
            raise ValueError(
                'FUCK! CHARLES EXPLODED!')

        new_instruction = (fncode, new_operands)
        new_instructions.append(new_instruction)

    return new_instructions
