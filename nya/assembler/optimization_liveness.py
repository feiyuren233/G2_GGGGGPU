from collections import defaultdict, deque
from isa import *
import copy


# global_liveset[bb_id] = local_liveset[local_inst_id] = {live registers}
def construct_liveset(cfg, instructions):
    bb_to_insts, _, bb_conn_forward, bb_conn_inverse = cfg

    num_bb = len(bb_to_insts)
    # global_liveset[bb_id] = local_liveset[local_inst_id] = {live registers}
    global_liveset = [None] * num_bb

    # As FIFO queue. append + popleft
    bb_queue = deque()
    bb_queue.append(num_bb - 1)

    # Tracking edges in the graph
    local_bb_conn_forward = copy.deepcopy(bb_conn_forward)

    # Topological Sort
    while bb_queue:
        bb_id = bb_queue.popleft()

        # Append ancestor bb_id into the queue
        for next_bb_id in bb_conn_inverse[bb_id]:
            local_bb_conn_forward[next_bb_id].remove(bb_id)
            if not local_bb_conn_forward[next_bb_id]:
                bb_queue.append(next_bb_id)

        # Compute the out_set for computing the liveset for bb_id
        out_set = set()
        for prev_bb_id in bb_conn_forward[bb_id]:
            if global_liveset[prev_bb_id] is None:
                raise ValueError('FUCK! CHARLES EXPLODED!')
            out_set.update(global_liveset[prev_bb_id][0])

        # Construct the local liveset
        global_liveset[bb_id] = construct_local_liveset(
            bb_to_insts[bb_id], instructions, out_set)

    return global_liveset


# liveset[local_inst_id] = {live registers before instruction}
def construct_local_liveset(insts_in_bb, instructions, end_liveset):
    num_insts = len(insts_in_bb)
    # live registers before the local_inst_id-th instruction in current bb
    # liveset[local_inst_id] = {live registers}
    liveset = [None] * (num_insts+1)

    liveset[num_insts] = copy.deepcopy(end_liveset)
    working_liveset = copy.deepcopy(end_liveset)
    for local_inst_id in reversed(range(num_insts)):
        instruction = instructions[insts_in_bb[local_inst_id]]

        # Remove rdest and add rsrc
        rdest, rsrc = find_registers(instruction)
        working_liveset.difference_update(rdest)
        working_liveset.update(rsrc)

        # Store into liveset
        liveset[local_inst_id] = copy.deepcopy(working_liveset)

    return liveset


# ({rdest}, {rsrc})
def find_registers(instruction):
    fncode, operands = instruction

    rdest = set()
    rsrc = set()
    if isinstance(fncode, RtypeFnCode):
        rd, ra, rb = operands
        rdest = {rd}
        rsrc = {ra, rb}
    elif isinstance(fncode, RFtypeFnCode):
        rd, ra, rb, rc = operands
        rdest = {rd}
        rsrc = {ra, rb, rc}
    elif isinstance(fncode, ItypeFnCode):
        rd, ra, _ = operands
        rdest = {rd}
        rsrc = {ra}
    elif isinstance(fncode, StypeFnCode):
        ra, rb, _ = operands
        rsrc = {ra, rb}
    elif isinstance(fncode, SBtypeFnCode):
        ra, rb, _ = operands
        if fncode == SBtypeFnCode.IF or fncode == SBtypeFnCode.ELIF:
            rsrc = {ra}
        else:
            rsrc = set()
    elif isinstance(fncode, UtypeFnCode):
        rd, _ = operands
        rdest = {rd}
    elif isinstance(fncode, UJtypeFnCode):
        rd, _ = operands
        rdest = {rd}
    else:
        raise ValueError(
            'FUCK! CHARLES EXPLODED!')

    return (rdest, rsrc)
