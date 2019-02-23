#include "qopcuaservernode.h"

#include <QOpcUaServer>
#include <QOpcUaProperty>
#include <QOpcUaBaseDataVariable>
#include <QOpcUaFolderObject>

// [STATIC]
UA_StatusCode QOpcUaServerNode::methodCallback(UA_Server        * server, 
	                                           const UA_NodeId  * sessionId, 
	                                           void             * sessionContext, 
	                                           const UA_NodeId  * methodId, 
	                                           void             * methodContext, 
	                                           const UA_NodeId  * objectId, 
	                                           void             * objectContext, 
	                                           size_t             inputSize, 
	                                           const UA_Variant * input, 
	                                           size_t             outputSize, 
	                                           UA_Variant       * output)
{
	qDebug() << "Method called";
	return UA_STATUSCODE_GOOD;
}

QOpcUaServerNode::QOpcUaServerNode(QOpcUaServerNode *parent) : QObject(parent)
{
	m_qopcuaserver = parent->m_qopcuaserver;
	m_nodeId       = UA_NODEID_NULL;
	// NOTE : type NodeId is null until class is registered in server
	Q_CHECK_PTR(m_qopcuaserver);
}

// PROTECTED
QOpcUaServerNode::QOpcUaServerNode(QOpcUaServer * server) : QObject(server)
{
	// This is a private constructor meant to be called only by QOpcUaServerNode
	// And its only purpose is to create the UA_NS0ID_OBJECTSFOLDER instance
	m_qopcuaserver = server;
	m_nodeId       = UA_NODEID_NULL;
	// NOTE : type NodeId is null until class is registered in server
	Q_CHECK_PTR(m_qopcuaserver);
}

QString QOpcUaServerNode::get_displayName() const
{
	Q_CHECK_PTR(m_qopcuaserver);
	Q_ASSERT(!UA_NodeId_isNull(&m_nodeId));
	if (UA_NodeId_isNull(&m_nodeId))
	{
		return QString();
	}
	// read description
	UA_LocalizedText outDisplayName;
	auto st = UA_Server_readDisplayName(m_qopcuaserver->m_server, m_nodeId, &outDisplayName);
	Q_ASSERT(st == UA_STATUSCODE_GOOD);
	Q_UNUSED(st)
	// return
	return QOpcUaTypesConverter::uaStringToQString(outDisplayName.text);
	// TODO : handle outDisplayName.locale
}

void QOpcUaServerNode::set_displayName(const QString & displayName)
{
	Q_CHECK_PTR(m_qopcuaserver);
	Q_ASSERT(!UA_NodeId_isNull(&m_nodeId));
	// convert to UA_LocalizedText
	QByteArray byteDisplayName = displayName.toUtf8(); // NOTE : QByteArray must exist in stack
    UA_LocalizedText uaDisplayName = UA_LOCALIZEDTEXT(trUtf8("en-US").toUtf8().data(), byteDisplayName.data());
	// set value
	auto st = UA_Server_writeDisplayName(m_qopcuaserver->m_server, m_nodeId, uaDisplayName);
	Q_ASSERT(st == UA_STATUSCODE_GOOD);
	Q_UNUSED(st)
	// TODO : handle locale
}

QString QOpcUaServerNode::get_description() const
{
	Q_CHECK_PTR(m_qopcuaserver);
	Q_ASSERT(!UA_NodeId_isNull(&m_nodeId));
	if (UA_NodeId_isNull(&m_nodeId))
	{
		return QString();
	}
	// read description
	UA_LocalizedText outDescription;
	auto st = UA_Server_readDescription(m_qopcuaserver->m_server, m_nodeId, &outDescription);
	Q_ASSERT(st == UA_STATUSCODE_GOOD);
	Q_UNUSED(st)
	// return
	return QOpcUaTypesConverter::uaStringToQString(outDescription.text);
	// TODO : handle outDescription.locale
}

void QOpcUaServerNode::set_description(const QString & description)
{
	Q_CHECK_PTR(m_qopcuaserver);
	Q_ASSERT(!UA_NodeId_isNull(&m_nodeId));
	// convert to UA_LocalizedText
	QByteArray byteDescription = description.toUtf8(); // NOTE : QByteArray must exist in stack
	UA_LocalizedText uaDescription = UA_LOCALIZEDTEXT(trUtf8("en-US").toUtf8().data(), byteDescription.data());
	// set value
	auto st = UA_Server_writeDescription(m_qopcuaserver->m_server, m_nodeId, uaDescription);
	Q_ASSERT(st == UA_STATUSCODE_GOOD);
	Q_UNUSED(st)
	// TODO : handle locale
}

quint32 QOpcUaServerNode::get_writeMask() const
{
	Q_CHECK_PTR(m_qopcuaserver);
	Q_ASSERT(!UA_NodeId_isNull(&m_nodeId));
	if (UA_NodeId_isNull(&m_nodeId))
	{
		return quint32();
	}
	// read writeMask
	UA_UInt32 outWriteMask;
	auto st = UA_Server_readWriteMask(m_qopcuaserver->m_server, m_nodeId, &outWriteMask);
	Q_ASSERT(st == UA_STATUSCODE_GOOD);
	Q_UNUSED(st)
	// return
	return outWriteMask;
}

void QOpcUaServerNode::set_writeMask(const quint32 & writeMask)
{
	Q_CHECK_PTR(m_qopcuaserver);
	Q_ASSERT(!UA_NodeId_isNull(&m_nodeId));
	// set value
	auto st = UA_Server_writeWriteMask(m_qopcuaserver->m_server, m_nodeId, writeMask);
	Q_ASSERT(st == UA_STATUSCODE_GOOD);
	Q_UNUSED(st)
}

QString QOpcUaServerNode::get_nodeId() const
{
	Q_CHECK_PTR(m_qopcuaserver);
	Q_ASSERT(!UA_NodeId_isNull(&m_nodeId));
	if (UA_NodeId_isNull(&m_nodeId))
	{
		return QString();
	}
	return QOpcUaTypesConverter::nodeIdToQString(m_nodeId);
}

QString QOpcUaServerNode::get_nodeClass() const
{
	Q_CHECK_PTR(m_qopcuaserver);
	Q_ASSERT(!UA_NodeId_isNull(&m_nodeId));
	if (UA_NodeId_isNull(&m_nodeId))
	{
		return QString();
	}
	// read nodeClass
	UA_NodeClass outNodeClass;
	auto st = UA_Server_readNodeClass(m_qopcuaserver->m_server, m_nodeId, &outNodeClass);
	Q_ASSERT(st == UA_STATUSCODE_GOOD);
	Q_UNUSED(st)
	// convert to QString
	return QOpcUaTypesConverter::nodeClassToQString(outNodeClass);
}

QPair<quint16, QString> QOpcUaServerNode::get_browseName() const
{
	Q_CHECK_PTR(m_qopcuaserver);
	Q_ASSERT(!UA_NodeId_isNull(&m_nodeId));
	if (UA_NodeId_isNull(&m_nodeId))
	{
		return QPair<quint16, QString>();
	}
	// read browse name
	UA_QualifiedName outBrowseName;
	auto st = UA_Server_readBrowseName(m_qopcuaserver->m_server, m_nodeId, &outBrowseName);
	Q_ASSERT(st == UA_STATUSCODE_GOOD);
	Q_UNUSED(st)
	// populate return value
	return QPair<quint16, QString>(outBrowseName.namespaceIndex, QOpcUaTypesConverter::uaStringToQString(outBrowseName.name));
}

void QOpcUaServerNode::set_browseName(const QBrowseName & browseName)
{
	Q_CHECK_PTR(m_qopcuaserver);
	Q_ASSERT(!UA_NodeId_isNull(&m_nodeId));
	// convert to UA_QualifiedName
	UA_QualifiedName bName;
	bName.namespaceIndex = browseName.first;
	bName.name           = QOpcUaTypesConverter::uaStringFromQString(browseName.second);
	// set value
	auto st = UA_Server_writeBrowseName(m_qopcuaserver->m_server, m_nodeId, bName);
	Q_ASSERT(st == UA_STATUSCODE_GOOD);
	Q_UNUSED(st)
}

QOpcUaProperty * QOpcUaServerNode::addProperty(const QString &strBrowseName/* = ""*/)
{
	return m_qopcuaserver->createInstance<QOpcUaProperty>(this, strBrowseName);
}

QOpcUaBaseDataVariable * QOpcUaServerNode::addBaseDataVariable(const QString &strBrowseName/* = ""*/)
{
	return m_qopcuaserver->createInstance<QOpcUaBaseDataVariable>(this, strBrowseName);
}

QOpcUaBaseObject * QOpcUaServerNode::addBaseObject(const QString &strBrowseName/* = ""*/)
{
	return m_qopcuaserver->createInstance<QOpcUaBaseObject>(this, strBrowseName);
}

QOpcUaFolderObject * QOpcUaServerNode::addFolderObject(const QString &strBrowseName/* = ""*/)
{
	return m_qopcuaserver->createInstance<QOpcUaFolderObject>(this, strBrowseName);
}

UA_NodeId QOpcUaServerNode::addMethodNodeInternal(QByteArray &byteMethodName, const size_t &nArgs, UA_Argument * inputArguments, UA_Argument * outputArgument)
{
    // add method node
    UA_MethodAttributes methAttr = UA_MethodAttributes_default;
    methAttr.executable     = true;
    methAttr.userExecutable = true;
    methAttr.description    = UA_LOCALIZEDTEXT((char *)"en-US",
                                               byteMethodName.data());
    methAttr.displayName    = UA_LOCALIZEDTEXT((char *)"en-US",
                                               byteMethodName.data());
    // create callback
    UA_NodeId methNodeId;
    auto st = UA_Server_addMethodNode(m_qopcuaserver->m_server,
                                      UA_NODEID_NULL,
                                      m_nodeId,
                                      UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                                      UA_QUALIFIEDNAME (1, byteMethodName.data()),
                                      methAttr,
                                      &QOpcUaServerNode::methodCallback,
                                      nArgs,
                                      inputArguments,
                                      1,
                                      outputArgument,
                                      this,
                                      &methNodeId);
    Q_ASSERT(st == UA_STATUSCODE_GOOD);
    Q_UNUSED(st);
    // return new methos node id
    return methNodeId;
}