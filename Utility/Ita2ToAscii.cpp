// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the ITA2-to-ASCII support logic.

	Revision History	:	2008-11-08 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/Ita2ToAscii.hpp>
#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

namespace {
// ////////////////////////////////////////////////////////////////////////////
//	Move into Utility/Ita2ToAscii.cpp.
/*
	From http://www.kekatos.com/teletype/gil/docs/smith--teletype-codes.txt

          USTTY                         ASCII translation
---------------------------           ---------------------
hex   binary    LTRS   FIGS              LTRS        FIGS
----  -----    -----  -----     |     ---------   ---------
0x00  00000    BLANK  BLANK     |     0x00  NUL   0x00  NUL
0x01  00001      E      3       |     0x45  E     0x33  3
0x02  00010      LF     LF      |     0x0A  LF    0x0A  LF
0x03  00011      A      -       |     0x41  A     0x2D  -
0x04  00100      SP     SP      |     0x20  SP    0x20  SP
0x05  00101      S     BELL     |     0x53  S     0x07  BEL
0x06  00110      I      8       |     0x49  I     0x38  8
0x07  00111      U      7       |     0x55  U     0x37  7
0x08  01000      CR     CR      |     0x0D  CR    0x0D  CR
0x09  01001      D      $       |     0x44  D     0x24  $
0x0A  01010      R      4       |     0x52  R     0x34  4
0x0B  01011      J      '       |     0x4A  J     0x27  '
0x0C  01100      N      ,       |     0x4E  N     0x2C  ,
0x0D  01101      F      !       |     0x46  F     0x21  !
0x0E  01110      C      :       |     0x43  C     0x3A  :
0x0F  01111      K      (       |     0x4B  K     0x28  (
0x10  10000      T      5       |     0x54  T     0x35  5
0x11  10001      Z      "       |     0x5A  Z     0x22  "
0x12  10010      L      )       |     0x4C  L     0x29  )
0x13  10011      W      2       |     0x57  W     0x32  2
0x14  10100      H      #       |     0x48  H     0x23  #
0x15  10101      Y      6       |     0x59  Y     0x36  6
0x16  10110      P      0       |     0x50  P     0x30  0
0x17  10111      Q      1       |     0x51  Q     0x31  1
0x18  11000      O      9       |     0x4F  O     0x39  9
0x19  11001      B      ?       |     0x42  B     0x3F  ?
0x1A  11010      G      &       |     0x47  G     0x26  &
0x1B  11011     FIGS   FIGS     |     0x0E  SO    0x0E  SO
0x1C  11100      M      .       |     0x4D  M     0x2E  .
0x1D  11101      X      /       |     0x58  X     0x2F  /
0x1E  11110      V      ;       |     0x56  V     0x3B  ;
0x1F  11111     LTRS   LTRS     |     0x0F  SI    0x0F  SI
*/
// ////////////////////////////////////////////////////////////////////////////
const char ITA2_To_AsciiMap[32][2] = {
	{ 0x00, 0x00 },	// ASCII = 0x00, LTRS = NUL, FIGS = NUL
	{ 0x45, 0x33 },	// ASCII = 0x01, LTRS = E,   FIGS = 3
	{ 0x0A, 0x0A },	// ASCII = 0x02, LTRS = LF,  FIGS = LF
	{ 0x41, 0x2D },	// ASCII = 0x03, LTRS = A,   FIGS = -
	{ 0x20, 0x20 },	// ASCII = 0x04, LTRS = SP,  FIGS = SP
	{ 0x53, 0x07 },	// ASCII = 0x05, LTRS = S,   FIGS = BEL
	{ 0x49, 0x38 },	// ASCII = 0x06, LTRS = I,   FIGS = 8
	{ 0x55, 0x37 },	// ASCII = 0x07, LTRS = U,   FIGS = 7
	{ 0x0D, 0x0D },	// ASCII = 0x08, LTRS = CR,  FIGS = CR
	{ 0x44, 0x24 },	// ASCII = 0x09, LTRS = D,   FIGS = $
	{ 0x52, 0x34 },	// ASCII = 0x0A, LTRS = R,   FIGS = 4
	{ 0x4A, 0x27 },	// ASCII = 0x0B, LTRS = J,   FIGS = '
	{ 0x4E, 0x2C },	// ASCII = 0x0C, LTRS = N,   FIGS = ,
	{ 0x46, 0x21 },	// ASCII = 0x0D, LTRS = F,   FIGS = !
	{ 0x43, 0x3A },	// ASCII = 0x0E, LTRS = C,   FIGS = :
	{ 0x4B, 0x28 },	// ASCII = 0x0F, LTRS = K,   FIGS = (
	{ 0x54, 0x35 },	// ASCII = 0x10, LTRS = T,   FIGS = 5
	{ 0x5A, 0x22 },	// ASCII = 0x11, LTRS = Z,   FIGS = "
	{ 0x4C, 0x29 },	// ASCII = 0x12, LTRS = L,   FIGS = )
	{ 0x57, 0x32 },	// ASCII = 0x13, LTRS = W,   FIGS = 2
	{ 0x48, 0x23 },	// ASCII = 0x14, LTRS = H,   FIGS = #
	{ 0x59, 0x36 },	// ASCII = 0x15, LTRS = Y,   FIGS = 6
	{ 0x50, 0x30 },	// ASCII = 0x16, LTRS = P,   FIGS = 0
	{ 0x51, 0x31 },	// ASCII = 0x17, LTRS = Q,   FIGS = 1
	{ 0x4F, 0x39 },	// ASCII = 0x18, LTRS = O,   FIGS = 9
	{ 0x42, 0x3F },	// ASCII = 0x19, LTRS = B,   FIGS = ?
	{ 0x47, 0x26 },	// ASCII = 0x1A, LTRS = G,   FIGS = &
	{ 0x0E, 0x0E },	// ASCII = 0x1B, LTRS = SO,  FIGS = SO
	{ 0x4D, 0x2E },	// ASCII = 0x1C, LTRS = M,   FIGS = .
	{ 0x58, 0x2F },	// ASCII = 0x1D, LTRS = X,   FIGS = /
	{ 0x56, 0x3B },	// ASCII = 0x1E, LTRS = V,   FIGS = ;
	{ 0x0F, 0x0F },	// ASCII = 0x1F, LTRS = SI,  FIGS = SI
};
// ////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
char Ita2LtrsCharToChar(unsigned int src_char)
{
	return(ITA2_To_AsciiMap[src_char % (1 << 5)][0]);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
char Ita2FigsCharToChar(unsigned int src_char)
{
	return(ITA2_To_AsciiMap[src_char % (1 << 5)][1]);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

