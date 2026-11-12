#include "../../../includes/class/Server.hpp"
//TOPIC <channel> [<topic>]
void	Server::handleTopic(Client& client, const Message& msg) {
	if (!topicValidation(client, msg))
		return ;

	const std::vector<std::string>& params = msg.getParser().params;
	const std::string& channelName = params[0];
	Channel& channel = _channels.find(channelName)->second;

	if (params.size() == 1) {//Display topic
		if (!channel.isTopicSet())
			return (saveOrSend(client, RPL_NOTOPIC(channelName)));//331

		else
			return (saveOrSend(client, RPL_TOPIC(channelName, channel.getTopic())));//332
	}
	else if (params.size() >= 2) {//Change topic
		if (channel.isModeActive(T) && !channel.isClientAnOperator(client))
			return (saveOrSend(client, ERR_CHANOPRIVSNEEDED(channelName)));//482
		channel.setTopic(params[1]);
		return (channel.broadcast(*this, RPL_TOPIC(channelName, params[1])));//332
	}
}

bool	Server::topicValidation(Client& client, const Message& msg) {
	if (!client.isRegistered())//451
		return (saveOrSend(client, ERR_NOTREGISTERED()), false);
	
	const std::vector<std::string>& params = msg.getParser().params;
	if (params.size() < 1)
		return (saveOrSend(client, ERR_NEEDMOREPARAMS(std::string("TOPIC"))), false);//461

	const std::string& channelName = params[0];
	std::map<std::string, Channel>::iterator it = _channels.find(channelName);
	if (it == _channels.end())
		return (saveOrSend(client, ERR_NOSUCHCHANNEL(channelName)), false);//403

	if (!it->second.getMemberByNick(client.getNickname()))
		return (saveOrSend(client, ERR_NOTONCHANNEL(channelName)), false);//442
	return (true);
}
