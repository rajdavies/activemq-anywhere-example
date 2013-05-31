/*
 * Copyright (C) Red Hat, Inc.
 * http://www.redhat.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.jboss.activemq.clients;

import javax.jms.BytesMessage;
import javax.jms.Connection;
import javax.jms.ConnectionFactory;
import javax.jms.JMSException;
import javax.jms.Message;
import javax.jms.MessageConsumer;
import javax.jms.Session;
import javax.jms.Topic;
import org.apache.activemq.ActiveMQConnectionFactory;
import org.apache.commons.cli.BasicParser;
import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Options;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class JMSConsumer {
    private static final Logger LOG = LoggerFactory.getLogger(JMSConsumer.class);

    private static final String URL = "tcp://localhost:61616";
    private static final String DESTINATION_TYPE = "Topic";
    private static final String DESTINATION_NAME = "audio";
    private static final int NUM_MESSAGES_TO_RECEIVE = 10000;


    public static void main(String args[]) {
        Connection connection = null;

        try {

            Options options = new Options();
            options.addOption("h", "help", false, "help:");
            options.addOption("url", true, "url for the broker to connect to");
            options.addOption("u", "username", true, "User name for connection");
            options.addOption("p", "password", true, "password for connection");
            options.addOption("d", "destination", true, "destination to send to");
            options.addOption("n", "number", true, "number of messages to send");
            options.addOption("delay", true, "delay between each send");

            CommandLineParser parser = new BasicParser();
            CommandLine commandLine = parser.parse(options, args);


            if (commandLine.hasOption("h")) {
                HelpFormatter helpFormatter = new HelpFormatter();
                helpFormatter.printHelp("OptionsTip", options);
                System.exit(0);
            }

            String url = commandLine.hasOption("host") ? commandLine.getOptionValue("host") :
                    URL;


            String userName = commandLine.hasOption("u") ? commandLine.getOptionValue("u") :
                    "admin";
            String password = commandLine.hasOption("p") ? commandLine.getOptionValue("p") : "admin";
            String destinationName = commandLine.hasOption("d") ? commandLine.getOptionValue("d") :
                    DESTINATION_NAME;
            int numberOfMessages = commandLine.hasOption("n") ? Integer.parseInt(commandLine.getOptionValue("n")) : NUM_MESSAGES_TO_RECEIVE;
            ;

            ConnectionFactory factory = new ActiveMQConnectionFactory(userName, password, url);

            connection = factory.createConnection();
            connection.start();
            Session session = connection.createSession(false, Session.AUTO_ACKNOWLEDGE);
            Topic topic = session.createTopic(destinationName);

            MessageConsumer consumer = session.createConsumer(topic);

            LOG.info("Start consuming " + numberOfMessages + " messages from " + topic.toString());

            for (int i = 0; i < numberOfMessages; i++) {
                Message message = consumer.receive();
                if (message != null) {
                    if (message instanceof BytesMessage) {
                        BytesMessage bytesMessage = (BytesMessage) message;
                        int len = (int) bytesMessage.getBodyLength();
                        byte[] data = new byte[len];
                        bytesMessage.readBytes(data);
                        String value = new String(data);


                        LOG.info("Got " + value);
                    } else {
                        LOG.info("Got a message " + message);
                    }
                }
            }

            consumer.close();
            session.close();
        } catch (Throwable t) {
            LOG.error("Error receiving message", t);
        } finally {

            if (connection != null) {
                try {
                    connection.close();
                } catch (JMSException e) {
                    LOG.error("Error closing connection", e);
                }
            }
        }
    }
}