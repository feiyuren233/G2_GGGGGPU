from collections import defaultdict
from isa import *
from codegen import *
import sys

try:
    from graphviz import Digraph
except:
    pass  # readline not available


# (bb_to_insts, bb_from_inst, bb_conn_forward, bb_conn_inverse)
def construct_cfg(instructions):
    # CFG DATA STRUCTURE
    # bb_to_insts[bb_id] = [list of inst_id]
    bb_to_insts = defaultdict(list)
    # bb_from_inst[inst_id] = bb_id
    bb_from_inst = [None] * len(instructions)
    # bb_conn_forward[bb_id] = [list of bb_id]
    bb_conn_forward = defaultdict(list)
    # bb_conn_inverse[bb_id] = [list of bb_id]
    bb_conn_inverse = defaultdict(list)

    # CONSTRUCTION
    inst_id = 0
    bb_id = 0
    cfg_diverging_root_bb_ids = []
    cfg_diverging_root_bb_tree_sinks = defaultdict(list)
    for instruction in instructions:
        fncode, _ = instruction

        # CFG diverges due to if
        if isinstance(fncode, SBtypeFnCode) and fncode == SBtypeFnCode.IF:
            # Keep track of the diverging root bb
            diverging_root_bb_id = bb_id
            cfg_diverging_root_bb_ids.append(diverging_root_bb_id)

            # Create a new bb
            bb_id += 1

            # Connect current bb to diverging_root_bb
            bb_conn_forward[diverging_root_bb_id].append(bb_id)
            bb_conn_inverse[bb_id].append(diverging_root_bb_id)
        elif isinstance(fncode, SBtypeFnCode) and fncode == SBtypeFnCode.ELSE:
            # Find the diverging root bb
            diverging_root_bb_id = cfg_diverging_root_bb_ids.pop()
            cfg_diverging_root_bb_ids.append(diverging_root_bb_id)

            # Add previous bb to the diverging root bb tree sinks
            cfg_diverging_root_bb_tree_sinks[diverging_root_bb_id].append(
                bb_id)

            # Create a new bb
            bb_id += 1

            # Connect current bb to diverging_root_bb
            bb_conn_forward[diverging_root_bb_id].append(bb_id)
            bb_conn_inverse[bb_id].append(diverging_root_bb_id)
        elif isinstance(fncode, SBtypeFnCode) and fncode == SBtypeFnCode.ENDIF:
            # Pop the diverging root bb
            diverging_root_bb_id = cfg_diverging_root_bb_ids.pop()

            # Add previous bb to the diverging root bb tree sinks
            cfg_diverging_root_bb_tree_sinks[diverging_root_bb_id].append(
                bb_id)

            # Create a new bb
            bb_id += 1

            # Connect current bb to predecessor diverging root bb tree sinks
            for sink_bb_id in cfg_diverging_root_bb_tree_sinks[diverging_root_bb_id]:
                bb_conn_forward[sink_bb_id].append(bb_id)
                bb_conn_inverse[bb_id].append(sink_bb_id)

        # Add current inst_id to current bb
        bb_to_insts[bb_id].append(inst_id)
        bb_from_inst[inst_id] = bb_id

        # Increment to next inst
        inst_id += 1

    return (bb_to_insts, bb_from_inst, bb_conn_forward, bb_conn_inverse)


def visualize_cfg(cfg, instructions, global_liveset):
    bb_to_insts, _, bb_conn_forward, bb_conn_inverse = cfg
    # global_liveset[bb_id] = local_liveset[local_inst_id] = {live registers}

    # print()
    # print()
    # print("bb_to_insts:", bb_to_insts)
    # print("bb_from_inst:", bb_from_inst)
    # print("bb_conn_forward:", bb_conn_forward)
    # print("bb_conn_inverse:", bb_conn_inverse)
    # print()
    # print()
    # print()

    if "graphviz" in sys.modules:
        dot = Digraph(comment='Control Flow Graph')

        # Node
        num_bb = len(bb_to_insts)
        for bb_id in range(num_bb):
            bb_content = ""
            for local_inst_id, inst_id in enumerate(bb_to_insts[bb_id]):
                bb_content += ("-- " + str(
                    global_liveset[bb_id][local_inst_id]) + "\n") if global_liveset else ""
                bb_content += "[" + str(inst_id) + "] "
                bb_content += str(
                    generate_assembly_code(instructions[inst_id]))
                bb_content += "\n"
            bb_content += ("-- " +
                           str(global_liveset[bb_id][-1]) + "\n") if global_liveset else ""
            dot.node(str(bb_id), label=bb_content, shape="box")

        # Forward Edge
        for bb_id in range(num_bb):
            for forward_conn_bb_id in bb_conn_forward[bb_id]:
                dot.edge(str(bb_id), str(forward_conn_bb_id))

        # Inverse Edge
        for bb_id in range(num_bb):
            for inverse_conn_bb_id in bb_conn_inverse[bb_id]:
                dot.edge(str(bb_id), str(inverse_conn_bb_id),
                         style="dotted", color="green", arrowhead="vee", arrowsize="0.5")

        # print(dot.source)
        dot.render('control_flow_graph.gv', view=False)
