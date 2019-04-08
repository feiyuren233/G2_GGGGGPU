from collections import defaultdict
import copy
import sys

try:
    from graphviz import Graph
except:
    pass  # readline not available


# logical_to_physical_mapping = reg name -> physical reg name
# (i_logical_to_physical_mapping, f_logical_to_physical_mapping)
def allocate_register(instructions, global_liveset):
    # global_liveset[bb_id] = local_liveset[local_inst_id] = {live registers}
    rig = contruct_rig(global_liveset)

    # decouple rig into (irig, frig, rrig)
    irig, frig, rrig = decouple_rig(rig)

    if rrig:
        print("Register Allocation Disabled!\n")
        return None

    # logical register to physical register mapping
    i_logical_to_physical_mapping, i_num_reg_used = register_k_color(
        irig, 64, 'i')
    f_logical_to_physical_mapping, f_num_reg_used = register_k_color(
        frig, 64, 'f')
    print()
    print(i_num_reg_used, "i registers used")
    print(f_num_reg_used, "f registers used")
    print()

    visualize_rig(irig, 'i', i_logical_to_physical_mapping)
    visualize_rig(frig, 'f', f_logical_to_physical_mapping)

    return (i_logical_to_physical_mapping, f_logical_to_physical_mapping)


# rig[reg_name] = {reg_names}
def contruct_rig(global_liveset):
    # global_liveset[bb_id] = local_liveset[local_inst_id] = {live registers}

    # rig[reg_name] = {reg_names}
    rig = defaultdict(set)

    # construct rig
    for local_liveset in global_liveset:
        for liveset in local_liveset:
            for reg in liveset:
                rig[reg].update(liveset.difference({reg}))

    return rig


# (irig, frig, rrig)
def decouple_rig(original_rig):
    ireg_set = set(
        filter(lambda reg_name: reg_name[0] == 'i', original_rig.keys()))
    freg_set = set(
        filter(lambda reg_name: reg_name[0] == 'f', original_rig.keys()))
    rreg_set = set(
        filter(lambda reg_name: reg_name[0] == 'r', original_rig.keys()))

    irig = defaultdict(set)
    for ireg in ireg_set:
        irig[ireg] = original_rig[ireg].intersection(ireg_set)

    frig = defaultdict(set)
    for freg in freg_set:
        frig[freg] = original_rig[freg].intersection(freg_set)

    rrig = defaultdict(set)
    for rreg in rreg_set:
        rrig[rreg] = original_rig[rreg].intersection(rreg_set)

    return (irig, frig, rrig)


def visualize_rig(rig, reg_type_name, logical_to_physical_mapping=None):
    # rig[reg_name] = {reg_names}

    if "graphviz" in sys.modules:
        dot = Graph(comment='Register Interference Graph - ' +
                    reg_type_name, strict=True)
        dot.graph_attr["splines"] = "false"

        # Node
        for reg, _ in rig.items():
            label_name = reg
            if logical_to_physical_mapping:
                label_name = label_name + \
                    " (" + logical_to_physical_mapping[reg] + ")"
            dot.node(reg, label=label_name)

        for reg, live_pair_regs in rig.items():
            for live_pair_reg in live_pair_regs:
                dot.edge(reg, live_pair_reg)

        # print(dot.source)
        dot.render('register_interference_graph_' +
                   reg_type_name + '.gv', view=False)


# logical_to_physical_mapping = reg name -> physical reg name
# num_physical_reg_needed
def register_k_color(rig, max_num_reg, reg_type_name):
    local_rig = copy.deepcopy(rig)

    # logical reg name -> physical reg name
    logical_to_physical_mapping = dict()

    # Problem
    # k-coloring the Register Interference Graph.
    # Algorithm
    # 1. Pick a node t that has less than k neighbors.
    # 2. Put t on a stack and remove it from the RIG.
    # 3. Repeat until the graph has only one node and color it.
    # 4. Pop and color one node from the stack, until the stack is empty.

    # stack: append and pop
    stack = []

    while local_rig:
        # Find a node with less than max_num_reg neighbors.
        # Put it on the stack and remove it from the rig.
        for reg, live_pair_regs in local_rig.items():
            if len(live_pair_regs) < max_num_reg:
                # Put it on stack
                stack.append(reg)
                # Remove it from rig
                for live_pair_reg in live_pair_regs:
                    local_rig[live_pair_reg].remove(reg)
                local_rig.pop(reg)
                # Break
                break

    local_rig = defaultdict(set)
    while stack:
        reg = stack.pop()
        # Construct edge with live pair regis in local_rig
        live_pair_regs = rig[reg].intersection(local_rig.keys())
        for live_pair_reg in live_pair_regs:
            local_rig[live_pair_reg].update({reg})
        local_rig[reg] = live_pair_regs
        # Assign color
        if len(live_pair_regs) >= max_num_reg:
            raise ValueError("Can't fit into", max_num_reg,
                             "physical", reg_type_name, "type registers.")
        live_pair_regs_physical_regs = set(
            map(lambda logical_reg: logical_to_physical_mapping[logical_reg], live_pair_regs))
        physical_reg_name = ""
        for reg_idx in range(max_num_reg):
            physical_reg_name = reg_type_name + str(reg_idx)
            if physical_reg_name not in live_pair_regs_physical_regs:
                break
        logical_to_physical_mapping[reg] = physical_reg_name

    num_physical_reg_needed = len(set(logical_to_physical_mapping.values()))

    return logical_to_physical_mapping, num_physical_reg_needed
