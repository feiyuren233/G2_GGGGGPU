// `include "gDefine.svh"

// typedef struct packed {
//     logic busy;
// } MapTableEntry_t;

// module MapTable #(
//     parameter MapTableID = 0
// )(
//     clk, rstn,

//     vra, vrb, vrc, vrd,
//     vrat, vrbt, vrct, vrdt,
//     gra, grb, grc, grd,
//     bra, brb, brc, brd

//     setVrd


// );
//     input VRegIdx_t vra, vrb, vrc, vrd;
//     input vrat, vrbt, vrct, vrdt;
//     output GRegIdx_t gra, grb, grc, grd;
//     output logic bra, brb, brc, brd;

//     MapTableEntry_t [`MAX_REG_THREAD - 1 : 0] mapTableI;
//     MapTableEntry_t [`MAX_REG_THREAD - 1 : 0] mapTableF;

//     always_ff @(posedge clk) begin
//         if (rstn == 0) begin
//             for (int i = 0; i < `MAX_REG_THREAD; i++) begin
//                 mapTableI[i].busy <= 0;
//                 mapTableF[i].busy <= 0;
//             end
//         end
//         else begin
//             bra <= vrat == 0 ? mapTableI[vra].busy : mapTableF[vra].busy;
//             brb <= vrbt == 0 ? mapTableI[vrb].busy : mapTableF[vrb].busy;
//             brc <= vrct == 0 ? mapTableI[vrc].busy : mapTableF[vrc].busy;
//             brd <= vrdt == 0 ? mapTableI[vrd].busy : mapTableF[vrd].busy;

//             gra <= vra + `MAX_REG_THREAD * MapTableID;
//             grb <= vrb + `MAX_REG_THREAD * MapTableID;
//             grc <= vrc + `MAX_REG_THREAD * MapTableID;
//             grd <= vrd + `MAX_REG_THREAD * MapTableID;
//         end
//     end

    

    
// endmodule