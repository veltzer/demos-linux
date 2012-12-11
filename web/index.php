<html>
	<body>
		<p>
		This is the web page for the linuxapi project.
		Read more about this project from it's
		<a href="https://github.com/veltzer/linuxapi">git page</a>.
		</p>
		<p>
		Mark Veltzer, <?php 
			$copyYear = 2010;
			$curYear = date('Y');
			echo $copyYear . (($copyYear != $curYear) ? '-' . $curYear : '');
		?>
		<a href="mailto:mark.veltzer@gmail.com">mark.veltzer@gmail.com</a>
		</p>
	</body>
</html>
