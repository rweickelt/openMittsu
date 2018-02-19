#ifndef OPENMITTSU_DATABASE_DATABASEWRAPPER_H_
#define OPENMITTSU_DATABASE_DATABASEWRAPPER_H_

#include <memory>

#include "src/database/Database.h"
#include "src/database/DatabasePointerAuthority.h"

namespace openmittsu {
	namespace database {

		class DatabaseWrapper : public Database {
		public:
			DatabaseWrapper(DatabasePointerAuthority const& databasePointerAuthority);
			DatabaseWrapper(DatabaseWrapper const& other);
			virtual ~DatabaseWrapper();

		private slots:
			void onDatabasePointerAuthorityHasNewDatabase();

			void onDatabaseContactChanged(openmittsu::protocol::ContactId const& identity);
			void onDatabaseGroupChanged(openmittsu::protocol::GroupId const& changedGroupId);
			void onDatabaseContactHasNewMessage(openmittsu::protocol::ContactId const& identity, QString const& messageUuid);
			void onDatabaseGroupHasNewMessage(openmittsu::protocol::GroupId const& group, QString const& messageUuid);
			void onDatabaseReceivedNewContactMessage(openmittsu::protocol::ContactId const& identity);
			void onDatabaseReceivedNewGroupMessage(openmittsu::protocol::GroupId const& group);
			void onDatabaseMessageChanged(QString const& uuid);
			void onDatabaseHaveQueuedMessages();
			void onDatabaseContactStartedTyping(openmittsu::protocol::ContactId const& identity);
			void onDatabaseContactStoppedTyping(openmittsu::protocol::ContactId const& identity);
		private:
			DatabasePointerAuthority const& m_databasePointerAuthority;
			std::weak_ptr<Database> m_database;

		public:
			void setupConnection();

			// Inherited via Database
			virtual openmittsu::protocol::GroupStatus getGroupStatus(openmittsu::protocol::GroupId const & group) const override;
			virtual openmittsu::protocol::ContactStatus getContactStatus(openmittsu::protocol::ContactId const & contact) const override;
			virtual openmittsu::protocol::ContactId getSelfContact() const override;
			virtual bool hasContact(openmittsu::protocol::ContactId const & identity) const override;
			virtual bool hasGroup(openmittsu::protocol::GroupId const & group) const override;
			virtual bool isDeleteted(openmittsu::protocol::GroupId const & group) const override;
			virtual openmittsu::protocol::MessageId storeSentContactMessageText(openmittsu::protocol::ContactId const & receiver, openmittsu::protocol::MessageTime const & timeCreated, bool isQueued, QString const & message) override;
			virtual openmittsu::protocol::MessageId storeSentContactMessageImage(openmittsu::protocol::ContactId const & receiver, openmittsu::protocol::MessageTime const & timeCreated, bool isQueued, QByteArray const & image, QString const & caption) override;
			virtual openmittsu::protocol::MessageId storeSentContactMessageLocation(openmittsu::protocol::ContactId const & receiver, openmittsu::protocol::MessageTime const & timeCreated, bool isQueued, openmittsu::utility::Location const & location) override;
			virtual openmittsu::protocol::MessageId storeSentContactMessageReceiptReceived(openmittsu::protocol::ContactId const & receiver, openmittsu::protocol::MessageTime const & timeCreated, bool isQueued, openmittsu::protocol::MessageId const & referredMessageId) override;
			virtual openmittsu::protocol::MessageId storeSentContactMessageReceiptSeen(openmittsu::protocol::ContactId const & receiver, openmittsu::protocol::MessageTime const & timeCreated, bool isQueued, openmittsu::protocol::MessageId const & referredMessageId) override;
			virtual openmittsu::protocol::MessageId storeSentContactMessageReceiptAgree(openmittsu::protocol::ContactId const & receiver, openmittsu::protocol::MessageTime const & timeCreated, bool isQueued, openmittsu::protocol::MessageId const & referredMessageId) override;
			virtual openmittsu::protocol::MessageId storeSentContactMessageReceiptDisagree(openmittsu::protocol::ContactId const & receiver, openmittsu::protocol::MessageTime const & timeCreated, bool isQueued, openmittsu::protocol::MessageId const & referredMessageId) override;
			virtual openmittsu::protocol::MessageId storeSentContactMessageNotificationTypingStarted(openmittsu::protocol::ContactId const & receiver, openmittsu::protocol::MessageTime const & timeCreated, bool isQueued) override;
			virtual openmittsu::protocol::MessageId storeSentContactMessageNotificationTypingStopped(openmittsu::protocol::ContactId const & receiver, openmittsu::protocol::MessageTime const & timeCreated, bool isQueued) override;
			virtual openmittsu::protocol::MessageId storeSentGroupMessageText(openmittsu::protocol::GroupId const & group, openmittsu::protocol::MessageTime const & timeCreated, bool isQueued, QString const & message) override;
			virtual openmittsu::protocol::MessageId storeSentGroupMessageImage(openmittsu::protocol::GroupId const & group, openmittsu::protocol::MessageTime const & timeCreated, bool isQueued, QByteArray const & image, QString const & caption) override;
			virtual openmittsu::protocol::MessageId storeSentGroupMessageLocation(openmittsu::protocol::GroupId const & group, openmittsu::protocol::MessageTime const & timeCreated, bool isQueued, openmittsu::utility::Location const & location) override;
			virtual openmittsu::protocol::MessageId storeSentGroupCreation(openmittsu::protocol::GroupId const & group, openmittsu::protocol::MessageTime const & timeCreated, bool isQueued, QSet<openmittsu::protocol::ContactId> const & members, bool apply) override;
			virtual openmittsu::protocol::MessageId storeSentGroupSetImage(openmittsu::protocol::GroupId const & group, openmittsu::protocol::MessageTime const & timeCreated, bool isQueued, QByteArray const & image, bool apply) override;
			virtual openmittsu::protocol::MessageId storeSentGroupSetTitle(openmittsu::protocol::GroupId const & group, openmittsu::protocol::MessageTime const & timeCreated, bool isQueued, QString const & groupTitle, bool apply) override;
			virtual openmittsu::protocol::MessageId storeSentGroupSyncRequest(openmittsu::protocol::GroupId const & group, openmittsu::protocol::MessageTime const & timeCreated, bool isQueued) override;
			virtual openmittsu::protocol::MessageId storeSentGroupLeave(openmittsu::protocol::GroupId const & group, openmittsu::protocol::MessageTime const & timeCreated, bool isQueued, bool apply) override;
			virtual void storeReceivedContactMessageText(openmittsu::protocol::ContactId const & sender, openmittsu::protocol::MessageId const & messageId, openmittsu::protocol::MessageTime const & timeSent, openmittsu::protocol::MessageTime const & timeReceived, QString const & message) override;
			virtual void storeReceivedContactMessageImage(openmittsu::protocol::ContactId const & sender, openmittsu::protocol::MessageId const & messageId, openmittsu::protocol::MessageTime const & timeSent, openmittsu::protocol::MessageTime const & timeReceived, QByteArray const & image, QString const & caption) override;
			virtual void storeReceivedContactMessageLocation(openmittsu::protocol::ContactId const & sender, openmittsu::protocol::MessageId const & messageId, openmittsu::protocol::MessageTime const & timeSent, openmittsu::protocol::MessageTime const & timeReceived, openmittsu::utility::Location const & location) override;
			virtual void storeReceivedContactMessageReceiptReceived(openmittsu::protocol::ContactId const & sender, openmittsu::protocol::MessageId const & messageId, openmittsu::protocol::MessageTime const & timeSent, openmittsu::protocol::MessageId const & referredMessageId) override;
			virtual void storeReceivedContactMessageReceiptSeen(openmittsu::protocol::ContactId const & sender, openmittsu::protocol::MessageId const & messageId, openmittsu::protocol::MessageTime const & timeSent, openmittsu::protocol::MessageId const & referredMessageId) override;
			virtual void storeReceivedContactMessageReceiptAgree(openmittsu::protocol::ContactId const & sender, openmittsu::protocol::MessageId const & messageId, openmittsu::protocol::MessageTime const & timeSent, openmittsu::protocol::MessageId const & referredMessageId) override;
			virtual void storeReceivedContactMessageReceiptDisagree(openmittsu::protocol::ContactId const & sender, openmittsu::protocol::MessageId const & messageId, openmittsu::protocol::MessageTime const & timeSent, openmittsu::protocol::MessageId const & referredMessageId) override;
			virtual void storeReceivedContactTypingNotificationTyping(openmittsu::protocol::ContactId const & sender, openmittsu::protocol::MessageId const & messageId, openmittsu::protocol::MessageTime const & timeSent) override;
			virtual void storeReceivedContactTypingNotificationStopped(openmittsu::protocol::ContactId const & sender, openmittsu::protocol::MessageId const & messageId, openmittsu::protocol::MessageTime const & timeSent) override;
			virtual void storeReceivedGroupMessageText(openmittsu::protocol::GroupId const & group, openmittsu::protocol::ContactId const & sender, openmittsu::protocol::MessageId const & messageId, openmittsu::protocol::MessageTime const & timeSent, openmittsu::protocol::MessageTime const & timeReceived, QString const & message) override;
			virtual void storeReceivedGroupMessageImage(openmittsu::protocol::GroupId const & group, openmittsu::protocol::ContactId const & sender, openmittsu::protocol::MessageId const & messageId, openmittsu::protocol::MessageTime const & timeSent, openmittsu::protocol::MessageTime const & timeReceived, QByteArray const & image, QString const & caption) override;
			virtual void storeReceivedGroupMessageLocation(openmittsu::protocol::GroupId const & group, openmittsu::protocol::ContactId const & sender, openmittsu::protocol::MessageId const & messageId, openmittsu::protocol::MessageTime const & timeSent, openmittsu::protocol::MessageTime const & timeReceived, openmittsu::utility::Location const & location) override;
			virtual void storeReceivedGroupCreation(openmittsu::protocol::GroupId const & group, openmittsu::protocol::ContactId const & sender, openmittsu::protocol::MessageId const & messageId, openmittsu::protocol::MessageTime const & timeSent, openmittsu::protocol::MessageTime const & timeReceived, QSet<openmittsu::protocol::ContactId> const & members) override;
			virtual void storeReceivedGroupSetImage(openmittsu::protocol::GroupId const & group, openmittsu::protocol::ContactId const & sender, openmittsu::protocol::MessageId const & messageId, openmittsu::protocol::MessageTime const & timeSent, openmittsu::protocol::MessageTime const & timeReceived, QByteArray const & image) override;
			virtual void storeReceivedGroupSetTitle(openmittsu::protocol::GroupId const & group, openmittsu::protocol::ContactId const & sender, openmittsu::protocol::MessageId const & messageId, openmittsu::protocol::MessageTime const & timeSent, openmittsu::protocol::MessageTime const & timeReceived, QString const & groupTitle) override;
			virtual void storeReceivedGroupSyncRequest(openmittsu::protocol::GroupId const & group, openmittsu::protocol::ContactId const & sender, openmittsu::protocol::MessageId const & messageId, openmittsu::protocol::MessageTime const & timeSent, openmittsu::protocol::MessageTime const & timeReceived) override;
			virtual void storeReceivedGroupLeave(openmittsu::protocol::GroupId const & group, openmittsu::protocol::ContactId const & sender, openmittsu::protocol::MessageId const & messageId, openmittsu::protocol::MessageTime const & timeSent, openmittsu::protocol::MessageTime const & timeReceived) override;
			virtual void storeMessageSendFailed(openmittsu::protocol::ContactId const & receiver, openmittsu::protocol::MessageId const & messageId) override;
			virtual void storeMessageSendDone(openmittsu::protocol::ContactId const & receiver, openmittsu::protocol::MessageId const & messageId) override;
			virtual void storeMessageSendFailed(openmittsu::protocol::GroupId const & group, openmittsu::protocol::MessageId const & messageId) override;
			virtual void storeMessageSendDone(openmittsu::protocol::GroupId const & group, openmittsu::protocol::MessageId const & messageId) override;
			virtual void storeNewContact(openmittsu::protocol::ContactId const & contact, openmittsu::crypto::PublicKey const & publicKey) override;
			virtual void storeNewGroup(openmittsu::protocol::GroupId const & groupId, QSet<openmittsu::protocol::ContactId> const & members, bool isAwaitingSync) override;
			virtual void sendAllWaitingMessages(openmittsu::dataproviders::SentMessageAcceptor & messageAcceptor) override;
			virtual std::unique_ptr<openmittsu::dataproviders::BackedContact> getBackedContact(openmittsu::protocol::ContactId const & contact, openmittsu::dataproviders::MessageCenter & messageCenter) override;
			virtual std::unique_ptr<openmittsu::dataproviders::BackedGroup> getBackedGroup(openmittsu::protocol::GroupId const & group, openmittsu::dataproviders::MessageCenter & messageCenter) override;
			virtual QSet<openmittsu::protocol::ContactId> getGroupMembers(openmittsu::protocol::GroupId const & group, bool excludeSelfContact) const override;
			virtual void enableTimers() override;

			// Contact Data
			virtual QString getFirstName(openmittsu::protocol::ContactId const& contact) const override;
			virtual QString getLastName(openmittsu::protocol::ContactId const& contact) const override;
			virtual QString getNickName(openmittsu::protocol::ContactId const& contact) const override;
			virtual openmittsu::protocol::AccountStatus getAccountStatus(openmittsu::protocol::ContactId const& contact) const override;
			virtual openmittsu::protocol::ContactIdVerificationStatus getVerificationStatus(openmittsu::protocol::ContactId const& contact) const override;
			virtual openmittsu::protocol::FeatureLevel getFeatureLevel(openmittsu::protocol::ContactId const& contact) const override;
			virtual int getColor(openmittsu::protocol::ContactId const& contact) const override;
			virtual int getContactCount() const override;
			virtual int getContactMessageCount(openmittsu::protocol::ContactId const& contact) const override;
			virtual QVector<QString> getLastMessageUuids(openmittsu::protocol::ContactId const& contact, std::size_t n) const override;

			virtual void setFirstName(openmittsu::protocol::ContactId const& contact, QString const& firstName) override;
			virtual void setLastName(openmittsu::protocol::ContactId const& contact, QString const& lastName) override;
			virtual void setNickName(openmittsu::protocol::ContactId const& contact, QString const& nickname) override;
			virtual void setAccountStatus(openmittsu::protocol::ContactId const& contact, openmittsu::protocol::AccountStatus const& status) override;
			virtual void setVerificationStatus(openmittsu::protocol::ContactId const& contact, openmittsu::protocol::ContactIdVerificationStatus const& verificationStatus) override;
			virtual void setFeatureLevel(openmittsu::protocol::ContactId const& contact, openmittsu::protocol::FeatureLevel const& featureLevel) override;
			virtual void setColor(openmittsu::protocol::ContactId const& contact, int color) override;

			// Group Data
			virtual QString getGroupTitle(openmittsu::protocol::GroupId const& group) const override;
			virtual QString getGroupDescription(openmittsu::protocol::GroupId const& group) const override;
			virtual bool getGroupHasImage(openmittsu::protocol::GroupId const& group) const override;
			virtual openmittsu::database::MediaFileItem getGroupImage(openmittsu::protocol::GroupId const& group) const override;
			virtual bool getGroupIsAwaitingSync(openmittsu::protocol::GroupId const& group) const override;
			virtual int getGroupCount() const override;
			virtual int getGroupMessageCount(openmittsu::protocol::GroupId const& group) const override;
			virtual QVector<QString> getLastMessageUuids(openmittsu::protocol::GroupId const& group, std::size_t n) const override;

			virtual void setGroupTitle(openmittsu::protocol::GroupId const& group, QString const& newTitle) override;
			virtual void setGroupImage(openmittsu::protocol::GroupId const& group, QByteArray const& newImage) override;
			virtual void setGroupMembers(openmittsu::protocol::GroupId const& group, QSet<openmittsu::protocol::ContactId> const& newMembers) override;
		};

	}
}

#endif // OPENMITTSU_DATABASE_DATABASEWRAPPER_H_
