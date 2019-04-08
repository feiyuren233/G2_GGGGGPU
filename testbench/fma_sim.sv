// `include "interfaces.svh"

// module tb_fma (
    
// );

//     logic clk, clken;
//     `FWORD a, b, c;
//     logic mode;
//     `FWORD out;

//     IFMA ifma(clk, clken, mode, a, b, c, out);

//     FMA fma(
//         .fma(ifma)
//     );

//     initial begin
//         clk = 0;
//         clken = 1;
//         a = 1.0;
//         b = 2.0;
//         c = 3.0;
//     end

//     // Veeeerilator doesn't support this?
//     // always #5 clk = !clk;



// endmodule