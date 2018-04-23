// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the ITA2-to-ASCII support logic.

	Revision History	:	2008-11-08 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__Utility__Ita2ToAscii_hpp__HH

#define HH__MLB__Utility__Ita2ToAscii_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file		Ita2ToAscii.hpp

	\brief	Include file for the ITA2-to-ASCII support logic.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
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

// ////////////////////////////////////////////////////////////////////////////
API_UTILITY char Ita2LtrsCharToChar(unsigned int src_char);
API_UTILITY char Ita2FigsCharToChar(unsigned int src_char);
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__Ita2ToAscii_hpp__HH

