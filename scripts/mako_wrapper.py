#!/usr/bin/python3

'''
This script is a wrapper for the mako templating system to be used from the command line,
build system or whatever.

It has command line parsing and ability to import your own variable definitions.
- It can process a single file, in the future this may change.
- It work in utf.
- It chmods the output file to be read only (to avoid accidentaly editing it).
- In itself this script defines no mako variables, the definitions are in an external module (attr).
- On any exception no output file is produced.

TODO:
- be able to override the encoding from the command line.
- make this script create the folder for the output file if that folder does not exist.
'''

###########
# imports #
###########
import sys # for argv, getdefaultencoding, exit
import mako # for exceptions
import mako.exceptions # for RickTraceback
import mako.template # for Template
import mako.lookup # for TemplateLookup
import os # for chmod, unlink
import os.path # for isfile
import attr # for Attr
import argparse # for ArgumentParser

#############
# functions #
#############

########
# code #
########

# constants
opt_input_encoding=sys.getdefaultencoding()
opt_output_encoding=sys.getdefaultencoding()

# get arguments from the command line
parser=argparse.ArgumentParser()
subparsers=parser.add_subparsers(title='subcommands', dest='subcommand')
subparser_process=subparsers.add_parser('process')
subparser_process.add_argument('--input', help='input file')
subparser_process.add_argument('--output', help='output file')
subparser_process.add_argument('--chmod', help='chmod the output?', default=True, action='store_false')
subparser_print=subparsers.add_parser('printmake')
subparser_print=subparsers.add_parser('getdeps')
parser.add_argument('-d','--debug', help='debug the script', default=False, action='store_true')
parser.add_argument('-p','--progress', help='report progress', default=False, action='store_true')
args=parser.parse_args()

if args.subcommand=='process':
	'''
	if there is any error, remove the output to prevent having
	bad output...
	'''
	try:
		'''
		We really need the unlink, even though we have *open a file
		for writing* later on which is supposed to truncate the file to 0
		since we chmod the output to be unwritable which means that the
		*open a file for writing* later would fail...
		'''
		if os.path.isfile(args.output):
			os.unlink(args.output)
		mylookup=mako.lookup.TemplateLookup(
			directories=['.'],
			input_encoding=opt_input_encoding,
			output_encoding=opt_output_encoding,
		)
		template=mako.template.Template(
			filename=args.input,
			lookup=mylookup,
			input_encoding=opt_input_encoding,
			output_encoding=opt_output_encoding,
		)
		file=open(args.output,'wb')
		try:
			attr.Attr.init()
		except Exception as e:
			print(e)
		file.write(template.render(attr=attr.Attr))
		file.close()
		if args.chmod:
			os.chmod(args.output,0o0444)
	except Exception as e:
		if os.path.isfile(args.output):
			os.unlink(args.output)
		'''
		the option that was chosen: print only errros in our file
		'''
		traceback=mako.exceptions.RichTraceback()
		for (filename, lineno, function, line) in traceback.traceback:
			if filename==args.input:
				print('{0}: error {1} in {2}, line {3}'.format(sys.argv[0], str(e), filename, lineno, function))
				print('{0}'.format(line))
		sys.exit(1)
		'''
		all kinds of options to handle errors:
		option1: assume we know the stack frame of the error
		traceback=mako.exceptions.RichTraceback()
		(filename, lineno, function, line)=traceback.traceback[-2]
		print('{0}: error {1} in {2}, line {3}'.format(sys.argv[0], str(e), filename, lineno, function))
		print('{0}'.format(line))
		sys.exit(1)

		option2: print full trace
		traceback=mako.exceptions.RichTraceback()
		for (filename, lineno, function, line) in traceback.traceback:
			print('File {0}, line {1}, in {2}'.format(filename, lineno, function))
			print(line)
		print('{0}: {1}'.format(str(traceback.error.__class__.__name__), traceback.error))

		option3: print a stupid error message
		print('something bad happened')

		option4: render the error (full stacktrace) using mako text error
		print(mako.exceptions.text_error_template().render())

		option5: raise the exception and make python print the stack trace
		raise e
		'''
if args.subcommand=='printmake':
	attr.Attr.init()
	for k in sorted(attr.Attr.__dict__.keys()):
		v=attr.Attr.__dict__[k]
		if not k.startswith('__') and type(v)==str:
			print('attr.{0}:={1}'.format(k, v))
if args.subcommand=='getdeps':
	print(attr.Attr.getdeps())
