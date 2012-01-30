/*
 * This is a simple example causing a core dump.
 *
 * The address "0" is not mapped into a processes virtual address space which is a good
 * thing since NULL is actually 0.
 *
 * 			Mark Veltzer
 */

int main(int argc, char **argv, char **envp)
{
	*(char *)0 = 0;
	return(0);
}
