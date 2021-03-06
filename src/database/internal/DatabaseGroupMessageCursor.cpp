#include "src/database/internal/DatabaseGroupMessageCursor.h"

#include "src/database/SimpleDatabase.h"
#include "src/exceptions/InternalErrorException.h"
#include "src/utility/Logging.h"

#include <QVariant>

namespace openmittsu {
	namespace database {
		namespace internal {

			using namespace openmittsu::dataproviders::messages;

			DatabaseGroupMessageCursor::DatabaseGroupMessageCursor(InternalDatabaseInterface* database, openmittsu::protocol::GroupId const& group) : DatabaseMessageCursor(database), GroupMessageCursor(), m_group(group) {
				//
			}

			DatabaseGroupMessageCursor::DatabaseGroupMessageCursor(InternalDatabaseInterface* database, openmittsu::protocol::GroupId const& group, openmittsu::protocol::MessageId const& messageId) : DatabaseMessageCursor(database), m_group(group) {
				if (!seek(messageId)) {
					throw openmittsu::exceptions::InternalErrorException() << "No message from group \"" << group.toString() << "\" and message ID \"" << messageId.toString() << "\" exists, invalid entry point.";
				}
			}

			DatabaseGroupMessageCursor::DatabaseGroupMessageCursor(InternalDatabaseInterface* database, openmittsu::protocol::GroupId const& group, QString const& uuid) : DatabaseMessageCursor(database), m_group(group) {
				if (!seekByUuid(uuid)) {
					throw openmittsu::exceptions::InternalErrorException() << "No message from group \"" << group.toString() << "\" and UUID \"" << uuid.toStdString() << "\" exists, invalid entry point.";
				}
			}

			DatabaseGroupMessageCursor::~DatabaseGroupMessageCursor() {
				//
			}

			openmittsu::protocol::GroupId const& DatabaseGroupMessageCursor::getGroupId() const {
				return m_group;
			}

			std::shared_ptr<GroupMessage> DatabaseGroupMessageCursor::getMessage() const {
				if (!isValid()) {
					throw openmittsu::exceptions::InternalErrorException() << "Can not create message wrapper for invalid message.";
				}

				return std::make_shared<DatabaseGroupMessage>(getDatabase(), m_group, getMessageId());
			}

			std::shared_ptr<DatabaseReadonlyGroupMessage> DatabaseGroupMessageCursor::getReadonlyMessage() const {
				if (!isValid()) {
					throw openmittsu::exceptions::InternalErrorException() << "Can not create message wrapper for invalid message.";
				}

				QSqlQuery query(getDatabase()->getQueryObject());
				query.prepare(QStringLiteral("SELECT `group_id`, `group_creator`, `apiid`, `uid`, `identity`, `is_outbox`, `is_read`, `is_saved`, `messagestate`, `sort_by`, `created_at`, `sent_at`, `received_at`, `seen_at`, `modified_at`, `group_message_type`, `body`, `is_statusmessage`, `is_queued`, `is_sent`, `caption` FROM `group_messages` WHERE `group_id` = :groupId AND `group_creator` = :groupCreator AND `uid` = :uid;"));
				bindWhereStringValues(query);
				query.bindValue(QStringLiteral(":uid"), QVariant(getMessageUuid()));
				if (!query.exec() || !query.isSelect() || !query.next()) {
					throw openmittsu::exceptions::InternalErrorException() << "Could not execute contact message query for table contact_messages. Query error: " << query.lastError().text().toStdString();
				}

				openmittsu::protocol::ContactId const contact(query.value(QStringLiteral("identity")).toString());
				openmittsu::protocol::MessageId const messageId(query.value(QStringLiteral("apiid")).toString());
				bool const isMessageFromUs = query.value(QStringLiteral("is_outbox")).toBool();
				openmittsu::protocol::MessageTime const createdAt(openmittsu::protocol::MessageTime::fromDatabase(query.value(QStringLiteral("created_at")).toLongLong()));
				openmittsu::protocol::MessageTime const sentAt(openmittsu::protocol::MessageTime::fromDatabase(query.value(QStringLiteral("sent_at")).toLongLong()));
				openmittsu::protocol::MessageTime const modifiedAt(openmittsu::protocol::MessageTime::fromDatabase(query.value(QStringLiteral("modified_at")).toLongLong()));
				bool const isQueued = query.value(QStringLiteral("is_queued")).toBool();
				bool const isSent = query.value(QStringLiteral("is_sent")).toBool();
				QString const uuid(query.value(QStringLiteral("uid")).toString());
				bool const isRead = query.value(QStringLiteral("is_read")).toBool();
				bool const isSaved = query.value(QStringLiteral("is_saved")).toBool();
				UserMessageState const messageState(UserMessageStateHelper::fromString(query.value(QStringLiteral("messagestate")).toString()));
				openmittsu::protocol::MessageTime const receivedAt(openmittsu::protocol::MessageTime::fromDatabase(query.value(QStringLiteral("received_at")).toLongLong()));
				openmittsu::protocol::MessageTime const seenAt(openmittsu::protocol::MessageTime::fromDatabase(query.value(QStringLiteral("seen_at")).toLongLong()));
				bool const isStatusMessage = query.value(QStringLiteral("is_statusmessage")).toBool();
				QString const caption(query.value(QStringLiteral("caption")).toString());
				openmittsu::dataproviders::messages::GroupMessageType const groupMessageType(openmittsu::dataproviders::messages::GroupMessageTypeHelper::fromString(query.value(QStringLiteral("group_message_type")).toString()));
				QString const body(query.value(QStringLiteral("body")).toString());

				openmittsu::database::MediaFileItem mediaItem;
				if ((groupMessageType == openmittsu::dataproviders::messages::GroupMessageType::AUDIO) || (groupMessageType == openmittsu::dataproviders::messages::GroupMessageType::FILE) || (groupMessageType == openmittsu::dataproviders::messages::GroupMessageType::IMAGE) || (groupMessageType == openmittsu::dataproviders::messages::GroupMessageType::VIDEO)) {
					mediaItem = getDatabase()->getMediaItem(uuid, MediaFileType::TYPE_STANDARD);
				} else {
					mediaItem = MediaFileItem(MediaFileItem::ItemStatus::UNAVAILABLE_NOT_IN_DATABASE, MediaFileType::TYPE_STANDARD);
				}

				auto drgm = std::make_shared<DatabaseReadonlyGroupMessage>(m_group, contact, messageId, isMessageFromUs, createdAt, sentAt, modifiedAt, isQueued, isSent, uuid, isRead, isSaved, messageState, receivedAt, seenAt, isStatusMessage, caption, groupMessageType, body, mediaItem);
				if (!drgm) {
					throw openmittsu::exceptions::InternalErrorException() << "Fetching a group message to readonly failed for group " << m_group.toString() << " and UUID " << getMessageUuid().toStdString() << "!";
				}
				return drgm;
			}

			QString DatabaseGroupMessageCursor::getWhereString() const {
				return QStringLiteral("`group_id` = :groupId AND `group_creator` = :groupCreator");
			}

			void DatabaseGroupMessageCursor::bindWhereStringValues(QSqlQuery& query) const {
				query.bindValue(QStringLiteral(":groupId"), QVariant(m_group.groupIdWithoutOwnerToQString()));
				query.bindValue(QStringLiteral(":groupCreator"), QVariant(m_group.getOwner().toQString()));
			}

			QString DatabaseGroupMessageCursor::getTableName() const {
				return QStringLiteral("group_messages");
			}

			QString DatabaseGroupMessageCursor::getMessageTypeField() const {
				return QStringLiteral("group_message_type");
			}

			void DatabaseGroupMessageCursor::deleteMessagesByAge(InternalDatabaseInterface* database, openmittsu::protocol::GroupId const& group, bool olderThanOrNewerThan, openmittsu::protocol::MessageTime const& timePoint) {
				QString whereAndOrderPart(QStringLiteral("AND `sort_by` %1 %2 ORDER BY `sort_by` %3"));
				if (olderThanOrNewerThan) {
					whereAndOrderPart = whereAndOrderPart.arg(QStringLiteral("<=")).arg(timePoint.getMessageTimeMSecs()).arg(QStringLiteral("ASC"));
				} else {
					whereAndOrderPart = whereAndOrderPart.arg(QStringLiteral(">=")).arg(timePoint.getMessageTimeMSecs()).arg(QStringLiteral("DESC"));
				}

				deletionHelper(database, group, whereAndOrderPart);
			}

			void DatabaseGroupMessageCursor::deleteMessagesByCount(InternalDatabaseInterface* database, openmittsu::protocol::GroupId const& group, bool oldestOrNewest, int count) {
				QString whereAndOrderPart(QStringLiteral("ORDER BY `sort_by` %1 LIMIT %2"));
				if (oldestOrNewest) {
					whereAndOrderPart = whereAndOrderPart.arg(QStringLiteral("ASC")).arg(count);
				} else {
					whereAndOrderPart = whereAndOrderPart.arg(QStringLiteral("DESC")).arg(count);
				}

				deletionHelper(database, group, whereAndOrderPart);
			}

			void DatabaseGroupMessageCursor::deletionHelper(InternalDatabaseInterface* database, openmittsu::protocol::GroupId const& group, QString const& whereAndOrderQueryPart) {
				database->transactionStart();
				QVector<QString> uuids;
				QString const selectQuery = QStringLiteral("SELECT `uid` FROM `group_messages` WHERE `group_id` = :groupId AND `group_creator` = :groupCreator %1").arg(whereAndOrderQueryPart);
				{
					QSqlQuery query(database->getQueryObject());
					if (!query.prepare(selectQuery)) {
						throw openmittsu::exceptions::InternalErrorException() << "Could not prepare group message enumeration query. SQL error: " << query.lastError().text().toStdString();
					}
					query.bindValue(QStringLiteral(":groupId"), QVariant(group.groupIdWithoutOwnerToQString()));
					query.bindValue(QStringLiteral(":groupCreator"), QVariant(group.getOwner().toQString()));
					if (!query.exec() || !query.isSelect()) {
						throw openmittsu::exceptions::InternalErrorException() << "Could not execute group message enumeration query for table group_messages. Query error: " << query.lastError().text().toStdString();
					}

					while (query.next()) {
						QString const uuid(query.value(QStringLiteral("uid")).toString());
						uuids.append(uuid);
					}
				}
				{
					QSqlQuery query(database->getQueryObject());
					QString const deleteQuery = QStringLiteral("DELETE FROM `group_messages` WHERE `uid` in (%1)").arg(selectQuery);
					if (!query.prepare(deleteQuery)) {
						throw openmittsu::exceptions::InternalErrorException() << "Could not prepare group message mass deletion query. SQL error: " << query.lastError().text().toStdString();
					}
					query.bindValue(QStringLiteral(":groupId"), QVariant(group.groupIdWithoutOwnerToQString()));
					query.bindValue(QStringLiteral(":groupCreator"), QVariant(group.getOwner().toQString()));
					if (!query.exec()) {
						throw openmittsu::exceptions::InternalErrorException() << "Could not execute group message mass deletion query for table group_messages. Query error: " << query.lastError().text().toStdString();
					}
				}
				{
					auto it = uuids.constBegin();
					auto const end = uuids.constEnd();
					for (; it != end; ++it) {
						database->removeAllMediaItems(*it);
					}
				}
				database->transactionCommit();

				auto it = uuids.constBegin();
				auto const end = uuids.constEnd();
				for (; it != end; ++it) {
					database->announceMessageDeleted(*it);
				}
			}

		}
	}
}
