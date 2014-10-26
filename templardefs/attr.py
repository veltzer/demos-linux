'''
Attributes for this project
'''

import datetime # for datetime
import subprocess # for check_output
import os.path # for join, expanduser
import glob # for glob
import socket # for gethostname
import configparser # for ConfigParser

class Attr(object):

	@classmethod
	def read_full_ini(cls, filename):
		ini_file=os.path.expanduser(filename)
		if os.path.isfile(ini_file):
			ini_config=configparser.ConfigParser()
			ini_config.read(ini_file)
			for section in ini_config.sections():
				for k,v in ini_config.items(section):
					setattr(cls, '{0}_{1}'.format(section, k), v)

	@classmethod
	def read_ini(cls, filename, sections):
		ini_file=os.path.expanduser(filename)
		if os.path.isfile(ini_file):
			ini_config=configparser.ConfigParser()
			ini_config.read(ini_file)
			for section in sections:
				for k,v in ini_config.items(section):
					setattr(cls, '{0}_{1}'.format(section, k), v)


	@classmethod
	def init(cls):
		# general # TODO: get homedir in python
		cls.general_current_year=datetime.datetime.now().year
		cls.general_homedir='/home/mark'
		#cls.general_hostname=subprocess.check_output(['hostname']).decode().rstrip()
		cls.general_hostname=socket.gethostname()
		cls.general_domainname=subprocess.check_output(['hostname','--domain']).decode().rstrip()

		# ini files
		cls.read_full_ini('~/.details.ini')

		# apt
		cls.apt_protocol='https'
		cls.apt_codename=subprocess.check_output(['lsb_release','--codename', '--short']).decode().rstrip()
		cls.apt_arch=subprocess.check_output('dpkg-architecture | grep -e ^DEB_BUILD_ARCH= | cut -d = -f 2', shell=True).decode().rstrip()
		cls.apt_archs='i386 {0} source'.format(cls.apt_arch)
		cls.apt_component='main'
		cls.apt_folder='apt'
		cls.apt_service_dir=os.path.join(cls.general_homedir, 'public_html/public', cls.apt_folder)
		cls.apt_except='50{0}'.format(cls.personal_slug)
		cls.apt_pack_list=glob.glob(os.path.join(cls.general_homedir, 'packages', '*.deb'))
		cls.apt_packlist=' '.join(cls.apt_pack_list)
		cls.apt_id=subprocess.check_output(['lsb_release','--id', '--short']).decode().rstrip()
		cls.apt_keyfile='public_key.gpg'
		cls.apt_apache_site_file='{0}.apt'.format(cls.personal_slug)

	@classmethod
	def getdeps(cls):
		return ' '.join([
			'templardefs/attr.py',
			os.path.expanduser('~/.details.ini'),
			'/etc/hostname',
		])
