# ActiveMQ Anywhere Example
=========================

Example using ActiveMQ and showing bridging MQTT -> &lt;Gateway> ---- AMQP ----- &lt;Broker> ---- JMS + WebSockets.


	

# Starting the MQTT Gateway
_________________________
1. cd activemq/gateway
2. run bin/gateway console

The gateway is an ActiveMQ broker, running in non-persistent mode with
transport connectors for MQTT ("mqtt://0.0.0.0:1883") and AMQP ("amqp://0.0.0.0:5672")

The gateway also has one camel route, that takes messages off a Topic (from MQTT) and puts them
on to a Queue (for AMQP)

# Starting the Broker
_________________________
1. cd activemq/broker
2. create a directory lib
4  download the QPid JMS AMQP client library - qpid-java-amqp-1-0-client-jms-0.20 - from http://qpid.apache.org/download.html to the lib/ directory and unpack the jars from the archive
4. run broker/broker console

The broker in an ActiveMQ broker, running in non-persistent mode (for simplicity) with 
transport connectors for openwire ("tcp://0.0.0.0:61616") and websockets ("ws://0.0.0.0:61614")

# Monitoring with hawtio
________________________
* e.g. start jetty - (jetty.sh)
* go to http://localhost:8080/hawtio

* connect remotely to the gateway:
	host=localhost
	port=8163
	path=api/jolokia

* connect remotely to the broker:
	host=localhost
	port=8161
	path=api/jolokia
	
Running the test clients

Shell1> mvn -P Consumer - Uses a JMS Subscriber to consume messages from the broker
Shell2> mvn -P Producer - Pushes MQTT messages to gateway

Running the Console

Deploy the camelone-web-1.0-SNAPSHOT.war to your favourite seb server deployment directory (rename it camelonedemo for ease of use)

go to http://localhost:8080/camelonedemo in your web browser
	

Setting up JBoss A-MQ
------------------------------
Firstly install the gateway and broker projects

1. From root directory:
	cd gateway
	mvn install
	
2. From root directory
	cd broker
	mvn install


The brokers are setup the same way in terms of transports as the ActiveMQ examples, except Camel is loaded via Karaf Features.
JBoss A-MQ is secure - use admin,adim

The Gateway:
transport connectors for MQTT ("mqtt://0.0.0.0:1883") and AMQP ("amqp://0.0.0.0:5672")
Jetty should be running from the gateway on 8163

cd jboss-amq/gateway
start bin/amq

load hawtio:
in the shell run:
JBossFuse:karaf@gateway> features:addurl mvn:io.hawt/hawtio-karaf/1.1/xml/features
JBossFuse:karaf@gateway> features:install hawtio

Now add the camel route for the gateway:

JBossFuse:karaf@gateway> features:addurl mvn:org.jboss.fuse.examples/gateway/1.0-SNAPSHOT/xml/features
JBossFuse:karaf@gateway> features:install jboss-example-broker



The broker:
transport connectors for MQTT ("mqtt://0.0.0.0:1883") and AMQP ("amqp://0.0.0.0:5672")
Jetty should be running from the gateway on 8161

cd jboss-amq/broker
start bin/amq

load hawtio:
in the shell run:
JBossFuse:karaf@broker> features:addurl mvn:io.hawt/hawtio-karaf/1.1/xml/features
JBossFuse:karaf@broker> features:install hawtio

JBossFuse:karaf@broker> features:addurl mvn:org.jboss.fuse.examples/broker/1.0-SNAPSHOT/xml/features
JBossFuse:karaf@broker> features:install jboss-example-gateway



Notes:

* all instructions assume you are executing from the top level directory of this project
* it is assumed you have Apache Maven installed, and that you are familiar with its usage
* assumes you are using Apache ActiveMQ 5.8.0 or later, or JBoss A-MQ 6.0.0 or later

