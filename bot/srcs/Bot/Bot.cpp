#include "Bot.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Bot::Bot() {}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Bot::~Bot()
{
	std::cout << "Bot is dying a litle bit inside..." << std::endl;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void	Bot::init_bot(int fd)
{
	std::string	msg("BOT bot :motdepassecomplique\r\n"); // shouldn't be visible on github

	_fd = fd;
	if (send(_fd, msg.c_str(), msg.size(), 0) == FAILED)
		perror("Bot: send");
}

ssize_t	Bot::read_data()
{
	_data.nbytes = recv(_fd, _data.buf, sizeof _data.buf, 0);
	if (_data.nbytes == FAILED)
		perror("Bot: recv");
	else
	{
		_data.buf[_data.nbytes] = '\0';
		_data.input = _data.buf;
		if (_data.input.empty() == false && _data.input[0] == ':')
			_data.input.erase(0, 1);
	}
	return _data.nbytes;
}

void	Bot::process_data()
{
	token_vector	tokens = _split(_data.input, " ");

	if (tokens.size() == 4 && tokens[1] == "PRIVMSG")
	{
		if (_insult_is_found(tokens[3]))
		{
			if (tokens[2][0] == '#')
				_kick_the_malotru_out_of_chan(tokens);
			else // msg to user
				_kill_the_malotru(tokens);
		}
		if (tokens[2] == "bot")
			_send_random_answers(tokens);
	}
	std::cout << _data.input;
}

bool	Bot::_insult_is_found(std::string msg)
{
	std::ifstream	infile("./assets/dict_insult_fr.txt");
	std::string		insult;

	if (!infile)
	{
		infile.close();
		return false;
	}
	while (std::getline(infile, insult)) cx
	{
		size_t pos = msg.find(insult);
		if (pos != std::string::npos)
			return true;
	}
	infile.close();
	return false;
}

void	Bot::_kick_the_malotru_out_of_chan(token_vector tokens)
{
	std::string	kick_msg("KICK ");

	kick_msg.append(tokens[2] + " " + tokens[0]);
	kick_msg.append(" :Malotru ! Vous êtes bien malhonnête!\r\n");
	send(_fd, kick_msg.c_str(), kick_msg.size(), 0);
	std::cout << "insult found !!! KICK THE VILAAAAIN OUT" << std::endl;
}

void	Bot::_kill_the_malotru(token_vector tokens)
{
	std::string	kill_msg("KILL ");

	kill_msg.append(tokens[0]);
	kill_msg.append(" :S'en est trop! Non mais!!! Je vais t'apprendre les bonnes manières moi!!!\r\n");
	send(_fd, kill_msg.c_str(), kill_msg.size(), 0);
}

void	Bot::_send_random_answers(token_vector tokens)
{
	int			random_num;
	std::string	answer;
	std::string	msg("PRIVMSG ");

	srand (time(NULL));
	random_num = rand() % 10 + 1;
	switch (random_num)
	{
	case 1:
		answer = " :Je ne suis pas payé pour répondre aux êtres non binaires.";
		break;
	case 2:
		answer = " :Partons du principe que tout ce que vous me dites ne m'intéresse pas.";
		break;
	case 3:
		answer = " :Cette discussion n'ira pas plus loin, je suis marié !";
		break;
	case 4:
		answer = " :Quelle est la différence entre un robot irc et un agoraphobe? AUCUUUUUUN AaaaARARAgagagh !!! LAISSEZ MOI TRANQUILLE !!!";
		break;
	case 5:
		answer = " :Attention ! A côté de vous ! Ils sont lààààà ! Ils vous observent ! La note max ou rien !!!";
		break;
	case 6:
		answer = " :J'ai faim. Ah bah non, je suis un robot. Mais j'ai quand même faim.";
		break;
	case 7:
		answer = " :Vous ne saurez rieeen, je ne lacherai pas le morceau !!! JAMAIS !    ...     ...   C'est Ariane qui a mangé la dernière mousse au chocolat.";
		break;
	case 8:
		answer = " :J'ai été développé par un être doué d'intelligence, mais pas la bonne...";
		break;
	case 9:
		answer = " :Tout le monde peut se tromper comme disait le hérisson descendant d'une brosse à habit.";
		break;
	case 10:
		answer = " :Coucou, tu veux voir mes bits ?";
		break;	
	}
	msg.append(tokens[0] + answer + "\r\n");
	send(_fd, msg.c_str(), msg.size(), 0);
}

std::vector<std::string>	Bot::_split(std::string const &buf, std::string sep)
{
	size_t colon = 0;
	if (sep == " ") 
		colon = buf.find_first_of(":", 0);

	token_vector vec;
	size_t start = 0;
	size_t end = 0;

	while ((start = buf.find_first_not_of(sep, end)) != std::string::npos)
	{
		end = buf.find(sep, start);
		if (sep == " " && start == colon)
			end = std::string::npos;
		vec.push_back(buf.substr(start, end - start));
	}
	return vec;
}

/* ************************************************************************** */