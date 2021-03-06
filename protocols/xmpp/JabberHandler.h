/*
 * Copyright 2010, Pier Luigi Fiorini. All rights reserved.
 * Distributed under the terms of the GPL v2 License.
 */
#ifndef _JABBER_HANDLER_H
#define _JABBER_HANDLER_H

#include <Notification.h>
#include <Path.h>
#include <String.h>

#include <libgloox/client.h>
#include <libgloox/chatstatehandler.h>
#include <libgloox/connectionlistener.h>
#include <libgloox/connectiontcpclient.h>
#include <libgloox/discohandler.h>
#include <libgloox/disco.h>
#include <libgloox/rostermanager.h>
#include <libgloox/loghandler.h>
#include <libgloox/logsink.h>
#include <libgloox/messagehandler.h>
#include <libgloox/messagesession.h>
#include <libgloox/messagesessionhandler.h>
#include <libgloox/messageeventhandler.h>
#include <libgloox/message.h>
#include <libgloox/presence.h>
#include <libgloox/vcardhandler.h>
#include <libgloox/vcardmanager.h>

#include <CayaProtocol.h>
#include <CayaConstants.h>

class BList;

class JabberHandler : public CayaProtocol, gloox::RosterListener, gloox::ConnectionListener,
								gloox::LogHandler, gloox::MessageSessionHandler,
								gloox::MessageHandler, gloox::MessageEventHandler,
								gloox::ChatStateHandler, gloox::VCardHandler {
public:
									JabberHandler();
	virtual							~JabberHandler();

			// CayaProtocol inheritance
	virtual	status_t				Init(CayaProtocolMessengerInterface*);

	virtual	status_t				Process(BMessage* msg);

	virtual	status_t				Shutdown();

	virtual	const char*				Signature() const;
	virtual	const char*				FriendlySignature() const;

	virtual	status_t				UpdateSettings(BMessage* msg);

	virtual	uint32					GetEncoding();

	virtual CayaProtocolMessengerInterface*
									MessengerInterface() const;

	virtual	uint32					Version() const;

			// Functions for gloox
			gloox::Client*			Client() const;
			void					HandleError(gloox::ConnectionError& e);

			// Callbacks for protocols
	virtual	void					OverrideSettings() = 0;
	virtual	BString					ComposeJID() const = 0;

protected:
			BString					fUsername;
			BString					fPassword;
			BString					fServer;
			BString					fResource;
			uint16					fPort;

private:
			CayaProtocolMessengerInterface*
									fServerMessenger;

			gloox::Client*			fClient;
			gloox::ConnectionTCPClient*
									fConnection;
			gloox::VCardManager*	fVCardManager;
			gloox::MessageSession*	fSession;

			gloox::JID				fJid;
			thread_id				fRecvThread;

			BPath					fCachePath;
			BPath					fAvatarCachePath;
			BMessage				fAvatarCache;
			BList*					fAvatars;

			void					_SendMessage(BMessage* msg);

			void					_MessageSent(const char* id, const char* subject,
												const char* body);

			void					_Notify(notification_type type, const char* title, const char* message);
			void					_NotifyProgress(const char* title, const char* message, float progress);

			status_t				_SetupAvatarCache();
			status_t				_SaveAvatarCache();
			void					_CacheAvatar(const char* id, const char* binval, size_t length);
			void					_AvatarChanged(const char*id, const char* filename);

			CayaStatus				_GlooxStatusToCaya(gloox::Presence::PresenceType type);

	virtual	void					onConnect();
	virtual	void					onDisconnect(gloox::ConnectionError);
	virtual	bool					onTLSConnect(const gloox::CertInfo&);
	virtual	void					onResourceBindError(const gloox::Error*);
	virtual	void					handleRoster(const gloox::Roster&);
	virtual	void					handleMessageSession(gloox::MessageSession* session);
	virtual	void					handleMessage(const gloox::Message& m, gloox::MessageSession*);
	virtual	void					handleMessageEvent(const gloox::JID& from, gloox::MessageEventType event);
	virtual	void					handleChatState(const gloox::JID& from, gloox::ChatStateType state);

	virtual	void					handleItemAdded(const gloox::JID&);
	virtual	void					handleItemSubscribed(const gloox::JID&);
	virtual	void					handleItemUnsubscribed(const gloox::JID&);
	virtual	void					handleItemRemoved(const gloox::JID&);
	virtual	void					handleItemUpdated(const gloox::JID&);
	virtual	void					handleRosterPresence(const gloox::RosterItem&,
														 const std::string&, gloox::Presence::PresenceType,
														 const std::string&);
	virtual	void					handleSelfPresence(const gloox::RosterItem&, const std::string&,
													   gloox::Presence::PresenceType, const std::string&);
	virtual	bool					handleSubscriptionRequest(const gloox::JID&, const std::string&);
	virtual	bool					handleUnsubscriptionRequest(const gloox::JID&, const std::string&);
	virtual	void					handleNonrosterPresence(const gloox::Presence&);
	virtual	void					handleRosterError(const gloox::IQ&);
	virtual	void					handleLog(gloox::LogLevel, gloox::LogArea, const std::string&);
	virtual	void					handleVCard(const gloox::JID&, const gloox::VCard*);
	virtual	void					handleVCardResult(gloox::VCardHandler::VCardContext,
													  const gloox::JID&,
													  gloox::StanzaError);
};

extern const char* kProtocolSignature;
extern const char* kProtocolName;

#endif	// _JABBER_HANDLER_H
