<!DOCTYPE html>
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
		<title>${tdefs.project_name}</title>
		<link rel="shortcut icon" type="image/x-icon" href="../static/favicon.ico"/>
		<link rel="icon" type="image/x-icon" href="../static/favicon.ico"/>
		<meta name="Description" content="${tdefs.project_long_description}"/>
		<meta name="Keywords" content="${tdefs.personal_fullname}, ${tdefs.personal_slug}, ${tdefs.project_name}, ${', '.join(tdefs.project_keywords)}"/>
		${tdefs.project_google_analytics_snipplet}
	</head>
	<body>
		<h1>Welcome to the <i>${tdefs.project_name}</i> web site</h1>
		<p>Some related <b>${tdefs.project_name}</b> project links:</p>
		<ul>
			<li>
			The <b>${tdefs.project_name}</b> github project is <a title="${tdefs.project_name} github project" href="${tdefs.project_website_source}">here</a>
			</li>
			<li>
			The <b>${tdefs.project_name}</b> web site for the project is <a title="${tdefs.project_name} web site" href="${tdefs.project_website}">here</a>
			</li>
			<li>
			The <b>${tdefs.project_name}</b> git clone link is <a title="${tdefs.project_name} git clone link" href="${tdefs.project_website_git}">here</a>

			</li>
			<li>
		</ul>
		<p>
			Copyright ${tdefs.personal_fullname}, ${tdefs.project_copyright_years}
			<a href="mailto:${tdefs.personal_email}">${tdefs.personal_email}</a>
		</p>
	</body>
</html>
