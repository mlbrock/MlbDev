#!/bin/perl

# #############################################################################
# #############################################################################
# Multiple Architecture Source Code Production System (MASCaPS) Version 3
# 		Makefile Specification List Construction Script
# #############################################################################
#
# File Name       : CreateSpecList.pl
#
# File Description: Construct MASCaPS makefile include lists.
#
# Revision History: 2015-03-27 --- Creation.
#                       Michael L. Brock
#
#       Copyright Michael L. Brock 2015 - 2015.
#
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
#
# #############################################################################

	$return_code    = -1;
	$debug_flag     = 0;
	$flattened_flag = 1;
	$path_sep_char  = "/";
	$prefix_name    = "";
	$base_file_name = "Context";
	$command_line   = join(" ", @ARGV);

	@KnownBaseList  = ("MakePrefix", "Context", "MakeSuffix");

	if (grep(/^\-\-?H$|\-\-?HELP$/i, @ARGV)) {
		EmitHelp();
	}

	if (grep(/^\-\-?FLAT(T(EN(ED)?)?)?$/i, @ARGV)) {
		$flattened_flag = 1;
		$path_sep_char  = "_";
		@ARGV           = grep(!/^\-\-?FLAT(T(EN(ED)?)?)?$/i, @ARGV);
	}

	if (grep(/^\-\-?PRE(FIX)?((_|\-)?NAME)?/i, @ARGV)) {
		@tmp_list = grep(/^\-\-?PRE(FIX)?((_|\-)?NAME)?/i, @ARGV);
		@ARGV     = grep(!/^\-\-?PRE(FIX)?((_|\-)?NAME)?/i, @ARGV);
		if ($#tmp_list > 0) {
			$tmp_error = "Only a single -prefix parameter is permitted, but " .
				($#tmp_list + 1) . " were found.";
			ReportFatalError($tmp_error);
		}
		elsif ($tmp_list[0] !~ /^\-\-?PRE(FIX)?((_|\-)?NAME)?=(.+)$/i) {
			$tmp_error = "The -prefix parameter ('" . $tmp_list[0] . "') does " .
				"not specify a directory name string.";
			ReportFatalError($tmp_error);
		}
		$tmp_string =  $tmp_list[0];
		$tmp_string =~ s/^\-\-?PRE(FIX)?((_|\-)?NAME)?=(.+)$/\4/i;
		if ($tmp_string =~ /\s+/) {
			$tmp_error = "The -prefix parameter base name string ('" .
				$tmp_string . "') contains whitespace characters.";
			ReportFatalError($tmp_error);
		}
		$prefix_name = $tmp_string;
	}

	if (grep(/^\-\-?BASE((_|\-)?NAME)?/i, @ARGV)) {
		@tmp_list = grep(/^\-\-?BASE((_|\-)?NAME)?/i, @ARGV);
		@ARGV     = grep(!/^\-\-?BASE((_|\-)?NAME)?/i, @ARGV);
		if ($#tmp_list > 0) {
			$tmp_error = "Only a single -base parameter is permitted, but " .
				($#tmp_list + 1) . " were found.";
			ReportFatalError($tmp_error);
		}
		elsif ($tmp_list[0] !~ /^\-\-?BASE((_|\-)?NAME)?=(.+)$/i) {
			$tmp_error = "The -base parameter ('", $tmp_list[0], "') does " .
				"not specify a base name string.";
			ReportFatalError($tmp_error);
		}
		$tmp_string =  $tmp_list[0];
		$tmp_string =~ s/^\-\-?BASE((_|\-)?NAME)?=(.+)$/\3/i;
		if ($tmp_string !~ /^[[:alnum:]_\-]+$/i) {
			$tmp_error = "The -base parameter base name string ('" .
				$tmp_string . "') contains characters which are neither " .
				"alphanumeric, an underscore, nor a hyphen.";
			ReportFatalError($tmp_error);
		}
		$base_file_name = $tmp_string;
	}

	if (grep(/(^\-\-?DEBUG(GING)?$)|(^\-\-?DBG$)/i, @ARGV)) {
		$debug_flag = 1;
		@ARGV       = grep(!/(^\-\-?DEBUG(GING)?$)|(^\-\-?DBG$)/i, @ARGV);
	}

	if ($prefix_name ne "") {
		$prefix_name =~ s/\/+/\//g;
		if (!(-e $prefix_name)) {
			$tmp_error = "Prefix directory '" . $prefix_name .
				"' does not exist.";
			ReportFatalError($tmp_error);
		}
		elsif (!(-d $prefix_name)) {
			$tmp_error = "Prefix directory '" . $prefix_name .
				"' exists, but is not a directory.";
			ReportFatalError($tmp_error);
		}
		if ($prefix_name !~ /\/$/) {
			$prefix_name .= "/";
		}
	}

	if ($#ARGV == 0) {
		$spec_string =  @ARGV[0];
		$spec_string =~ s/^\s+|\s+$//g;
		if (($spec_string eq "") || ($spec_string eq "/") ||
			($spec_string eq "\\") || ($spec_string eq ".")) {
			$tmp_error = "The MASCaPS spec string is effectively empty ('" .
				$spec_string . "').";
			ReportFatalError($tmp_error);
		}
		elsif ($spec_string =~ /\s+/) {
			$tmp_error = "The MASCaPS spec string contains whitespace ('" .
				$spec_string . "').";
			ReportFatalError($tmp_error);
		}
		else {
			$this_hostname =  `/bin/hostname -s`;
			if ($? == -1) {
				$tmp_error = "ERROR: Attempt to determine host name failed: " . $!;
				ReportFatalError($tmp_error);
			}
			else {
				$this_hostname =~ s/^\s+|\s+$//g;
				$this_username =  `/usr/bin/whoami`;
				if ($? == -1) {
					$tmp_error = "Attempt to determine user name failed: " . $!;
					ReportFatalError($tmp_error);
				}
				else {
					$this_username =~ s/^\s+|\s+$//g;
					@spec_list     =  split(/\s*\/\s*/, $spec_string);
					unshift(@spec_list, "");
					$work_string   =  "";
					$part_string   =  "";
					$out_string    =  "";
					$all_string    =  "";
					@combo_list    = (
						"",
						".".$this_hostname,
						".".$this_username,
						".".$this_username.".".$this_hostname,
					);
					%part_hash = ();
					for $this_spec (@spec_list) {
						$seg_count = 1;
						if ($this_spec ne "") {
							$work_string .= $this_spec . $path_sep_char;
							$part_string  = $this_spec . $path_sep_char;
							if (($part_string ne $work_string) &&
								 (!defined($part_hash[$part_string]))) {
								$seg_count = 2;
							}
						}
						for ($count_1 = 0; $count_1 < $seg_count; ++$count_1) {
							$seg_string = (!$count_1) ? $work_string : $part_string;
							for $this_combo (@combo_list) {
								$this_file = $prefix_name.
									$seg_string.$base_file_name.$this_combo.".mk";
								if (CheckComboFile($seg_string, $this_combo)) {
									$out_string .= (($out_string eq "") ? "" : " ").
										$this_file;
								}
								$all_string .= (($all_string eq "") ? "" : " ").
									$this_file;
							}
						}
					}
					$return_code = 0;
					if ($debug_flag) {
						print STDERR "=" x 79; print STDERR "\n";
						print STDERR "=" x 79; print STDERR "\n";
						print STDERR "Debugging Info:\n";
						print STDERR "-" x 79; print STDERR "\n";
						print STDERR "Debugging Flag: ",
							(($debug_flag) ? "TRUE" : "FALSE"), "\n";
						print STDERR "Flattened Flag: ",
							(($flattened_flag) ? "TRUE" : "FALSE"), "\n";
						print STDERR "Path Sep Chat : ", $path_sep_char, "\n";
						print STDERR "Prefix Path   : ", $prefix_name, "\n";
						print STDERR "Base String   : ", $base_file_name, "\n";
						print STDERR "-" x 79; print STDERR "\n";
						@tmp_list = split(/ /, $all_string);
						print STDERR "MASCaPS Include Makefiles (All) :";
						printf(STDERR "%5d\n", $#tmp_list + 1);
						print STDERR "------- ------- --------- ------ -----\n";
						print STDERR join("\n", @tmp_list), "\n";
						print STDERR "-" x 79; print STDERR "\n";
						@tmp_list = split(/ /, $out_string);
						print STDERR "MASCaPS Include Makefiles (Used):";
						printf(STDERR "%5d\n", $#tmp_list + 1);
						print STDERR "------- ------- --------- ------ -----\n";
						print STDERR join("\n", @tmp_list), "\n";
						print STDERR "=" x 79; print STDERR "\n\n";
					}
					print $out_string, "\n";
				}
			}
		}
	}
	elsif ($#ARGV > -1) {
		$tmp_error = "Unrecognized extra parameters were specified on " .
			"command line ('" . $command_line . "').";
		ReportFatalError($tmp_error);
	}
	else {  
		$tmp_error = "The MASCaPS spec string was not specified on the " .
			"command line.";
		ReportFatalError($tmp_error);
	}

	exit($return_code);
# #############################################################################

# #############################################################################
sub CheckComboFile {
	local($in_work_string, $in_combo) = @_;

	for $this_base (@KnownBaseList) {
		if (-e $prefix_name.$in_work_string.$this_base.$in_combo.".mk") {
			return(1);
		}
	}

	return(0);
}
# #############################################################################

# #############################################################################
sub EmitHelp {
	print "Help request with \'-HELP\' noted . . .\n\n";

	$tmp_string =  $0;
	$tmp_string =~ s/^(.+)\/(.+)$/\2/;

	print "   ", $tmp_string, " [-flat] [-base=<string>] [-prefix=<path>] ",
		"[-debug] <spec>\n\n";
	print "      -flat\n",
			"         Specifies that the individual elements of the\n",
			"         <spec> parameter are to be separated by a single\n",
			"         underscore character ('_').\n",
			"\n",
			"         If not specified, the concatenated elements of the\n",
			"         <spec> parameter will be separated with a single\n",
			"         directory path name slash character.\n",
			"\n";
	print "      -prefix=<path>\n",
			"         Specifies the path name to be used as the prefix\n",
			"         of generated path names.\n",
			"\n",
			"         If not specified no prefix will be used.\n",
			"\n";
	print "      -base=<string>\n",
			"         Specifies the base file name to be used as the basis\n",
			"         of the make file name when the concatenated elements\n",
			"         of the <spec> parameter are emitted.\n",
			"\n",
			"         If not specified the default value will be used which\n",
			"         is the string 'Context'.\n",
			"\n";
	print "      <spec>\n",
			"         Specifies the directory path name which is to be used\n",
			"         to specify elements to be concatenated to produce the\n",
			"         MASCaPS target make include files.\n",
			"\n";
	print "      -debug\n",
			"         If specified, this program will emit debbugging info.\n",
			"\n";

	exit(0);
}
# #############################################################################

# #############################################################################
sub ReportFatalError {
	local($error_text) = @_;

	print "ERROR: ", $error_text, "\n";

	exit(-1);
}
# #############################################################################

