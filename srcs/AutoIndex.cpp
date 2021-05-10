#include "../includes/AutoIndex.hpp"
#include "../includes/Utils.hpp"

void	make_dir_list(const char *path)
{
	DIR				*dir_ptr		=	NULL;
	struct dirent	*file			=	NULL;
	int				fd;

	if (ft::is_dir(path))
	{
		fd = open("./autoresult.html", O_CREAT | O_RDWR, 0644);
		dir_ptr = opendir(path);
		while ((file = readdir(dir_ptr)) != NULL)
		{
			string	total;
			string	dfname;
			string	url = path;
			url += '/';
			if (ft::strcmp(file->d_name, ".") == 0)
			{
				continue;
			}
			total += "<A href = \"";
			url += file->d_name;
			total += url;
			total += "\" target = \"self\">";
			total += file->d_name;
			total += "</A>";
			total += "<br>";
			write(fd, total.c_str(), strlen(total.c_str()));
		}
		close(fd);
		closedir(dir_ptr);
	}
}

int		main(void)
{
	make_dir_list(".");
}
