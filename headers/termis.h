#ifndef Termis
#define Termis


#include <termios.h>
#include <unistd.h>

/**
 * @brief      class allows switch terminal mode with non canonical and default
 */
class Termis
{
private:
	struct termios oldt, newt;
	void scan_std()
	{
		tcgetattr(STDIN_FILENO, &oldt);
		newt = oldt;
		newt.c_lflag &= ~(ICANON|ECHO);
	}
	void set_nstd()
	{
		tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	}
	void set_ostd()
	{
		tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	}
public:
	/**
	 * @brief      Save your terminal attributes to private array and sets the new attributes to uncanonical input mode
	 */
	void start()
	{
		scan_std();
		set_nstd();
	};

	/**
	 * @brief      Sets the old attributes to canonical input mode
	 */
	void stop()
	{
		set_ostd();
	}
	/**
	 * @brief      Constructs a new instance and call start() function
	 */
	Termis()
	{
		start();	
	};
	~Termis()
	{
		stop();
	};

};

#endif