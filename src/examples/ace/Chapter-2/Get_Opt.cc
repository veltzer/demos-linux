#include <ace/OS_NS_string.h>
#include <ace/Get_Opt.h>
#include <ace/Log_Msg.h>

/*
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */

//
// options are: -f file FileName
// -g
// -h
// -l parameter
// --cool_option - we get 0 into the switch
// --the_answer - we get 42 into the switch
//
//
int ACE_TMAIN(int argc, ACE_TCHAR *argv[]) { // Example for a long option without a corresponding short option.
	// Just put some context here so the following compiles and runs.
	static const ACE_TCHAR options[] = ACE_TEXT(":f:l:gh");

	ACE_Get_Opt cmd_opts(argc, argv, options);

	cmd_opts.long_option(ACE_TEXT("cool_option"));
	cmd_opts.long_option(ACE_TEXT("the_answer"), 42);

	int option;
	ACE_TCHAR config_file[MAXPATHLEN];
	ACE_OS_String::strcpy(config_file, ACE_TEXT("HAStatus.conf"));
	while ((option = cmd_opts()) != EOF) {
		switch (option) {
		case 'f':
			ACE_OS_String::strncpy(config_file, cmd_opts.opt_arg(), MAXPATHLEN);
			ACE_DEBUG((LM_DEBUG, "-f: The new config_file: %s\n", config_file));
			break;

		case 'g':
			ACE_DEBUG((LM_DEBUG, "-g: is selected\n"));
			break;

		case 'h':
			ACE_DEBUG((LM_DEBUG, "-h: is selected\n"));
			break;

		case 'l':
			ACE_DEBUG((LM_DEBUG, "-l: is selected we ignore the optional parameter.\n"));
			break;

		case 0:
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("Yes, very cool.\n")));
			break;

		case 42:
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("the_answer is 42\n")));
			break;

		case ':':
			ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("-%c requires an argument\n"), cmd_opts.opt_opt()), -1);


		default:
			ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("Parse error.\n")), -1);
		}
	}

	return(0);
}
