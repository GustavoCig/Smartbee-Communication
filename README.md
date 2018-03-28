# Smartbee-Comunication

Short installation guide on how to get your smartbee webservice up and running.
If you plan on hosting your webservice in a VM, please proceed to the topic below [VM Configuration](https://github.com/GustavoCig/Smartbee-Comunication#vm-configuration), else, proceed to the topic [Java Configuration](https://github.com/GustavoCig/Smartbee-Comunication#java-configuration).

## VM Configuration

This topic mostly covers how to correct a VM's resolution inside **VirtualBox**, in case it's restrictive and impossible to change manually.

First install VirtualBox itself inside your VM, thus, installing the following drivers.
```
sudo apt-get update
sudo apt-get install virtualbox
```

Then install VirtualBox's Guest Additions:
With the VM started, go to the **toolbar at the top of the VM's window** and into **Options**
**Mount the Guest Additions disk** then go inside the disk through the VM, as if it was a regular USB or CD/DVD being used in a regular computer.
Then find and execute the correct script for your VM's OS, in case of it being a Linux distro(Ubuntu):
```
cd /media/{user name}/VBox.../
sudo sh ./VBoxLinuxAdditions.sh
``` 
Finally, all that's left is to restart your VM and then see if it's resolution is configurable.
You may need to change the resolution by change the actual VM's screen resolution instead of using VirtualBox's toolbar.

## Java Configuration

Since Smartbee's webservice is developed in Java, the Java version that's going to be used to host the webservice needs to be the **same version used to build the .war file.**
Otherwise, an major.minor version error might be thrown during execution.

Therefore, install the appropriate version, but, for the sake of this tutorial, we are going to follow through the installation steps for a Linux distro(Ubuntu):
```
sudo add-apt-repository ppa:webupd8team/java
sudo apt-get update
sudo apt-get install oracle-java{version-number}-installer
```
- Java 7's installation using the method above threw an exception, interrupting it.
Furthermore, if you wish to use Java 7, you may need to also use an older version of JBoss.
Since at least one feature of newer versions of JBoss isn't supported by Java 7 and older versions
- Java 8's installation was successful.
- Java 9's installation threw an error at the end, however seemingly was installed correctly.

Verify, after the procedure above, if java installed correctly.
```
java -version
```

## JBoss Installation

To host this service we've been using **JBoss** as the service's server. Other servers might be just as usable as JBoss, however, no tests were made.

First download JBoss:
```
wget http://download.jboss.org/wildfly/{version name}/{version name}.tar.gz
executar .tar
sudo tar -xvzf {version name}.tar.gz
```

Then we have to change some variables inside JBoss configurations:
```
cd {version name}/bin/
{text editor of choice} standalone.conf
```

Inside standalone.conf we might need to change JAVA/JAVA_HOME environment variable.
Simply point one of these two to the correct java directory.
```
JAVA_HOME="/usr"
```
or
```
JAVA="/usr/bin/java"
```
**only one of these needs to be set**

Then execute the standalone script:
```
sudo sh ./standalone.sh
```
If no exceptions are thrown, your server just went online.

- This procedure was tested with many versions of JBoss, from Wildfly 8 to Wildfly 11, all successful.

## Building .war file.

- Building process was tested with Eclipse's Java EE IDE.

The process is pretty simple.
Inside the IDE go your project's folder, select it then, either through the toolbar or by right-clicking the folder's name, select the option **Export**.
From the list that appears in the following windor, choose the Web option, then WAR file option.
All that's left is to choose in which directory you want your .war file created.

## Transfering .war file.

We are going to base ourselves that FTP is needed to transfer your .war file to your server.
If it's not necessary or your server already has a FTP Client installed, then continue to the next topic [Deploying .war](ttps://github.com/GustavoCig/Smartbee-Comunication#deploying-.war)

- If you are running you server inside a VM, you might need to make some changes to it's network configuration to be able to access it directly through FTP:

- Go to the configurations option of your VM, then Network, select Brigde Mode from the dropdown menu, restart your VM if it was running.
Then try to contact your VM(ping).

Now we are ready to install our FTP software.
In this tutorial we used the VSFTPD client.
```
sudo apt-get install vsftpd
sudo service vsftpd start
```
We need to change vsftp configuration if we want to allow our server to receive files(Write permission).
```
{text editor of choice} /etc/vsftpd.conf
```
Search for the write permissions and change or uncomment the line.

Now connect to your server thorugh FTP and trasnfer the .war file.

# Deploying .war

With the server online, we need to access it's CLI:
```
cd {server file's path}/bin/
sudo sh ./jboss-cli.sh
```
Once inside, all that's left is to use the deploy command followed by your .war file's path:
```
deploy {path}/webservice.war
```

With this you are done configuring your server.
Now all that's left is for you to seed your webservice's database, since the commands don't function or return empty values in a unseeded database.

## Database

The database used by our service is MySQL.
To install it, use these commands:
```
sudo apt-get install mysql-server
```
- Optional: Install the tool mysql-workbench, which allows to more easily manage the database and it's data.
```
sudo apt-get install mysql-workbench
```
- Configure an user and it's password during installation.

Access to the database is done using the following command:
```
mysql>mysql -u {username} -p
mysql>{password for the user}
```

Inside the CLI create a databse named smartbee_projeto.
```
CREATE DATABASE smartbee_projeto;
```

All that's left is to configure and seed the database, which can be done by using the scripts also located in this repository:
- config.sql
- test_seed.sql
In this order.
```
mysql>source {file path}/{file name}.sql;
```

## Postman
Postman is a HTTP client which can be used to test the service and it's function.
To install it run the following commands:
```
wget https://dl.pstmn.io/download/latest/linux64 -O postman.tar.gz
sudo tar -xzf postman.tar.gz -C /opt
sudo ln -s /opt/Postman/Postman /usr/bin/postman
```
