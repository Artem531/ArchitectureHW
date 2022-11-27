# Пример первого домашнего задания по курсу Архитектура программных систем

# Подготовка к запуску
## Настройка пользователя и базы в MySQL
sudo mysql
CREATE USER 'stud'@'localhost' IDENTIFIED BY 'stud';
GRANT ALL PRIVILEGES ON * . * TO 'stud'@'localhost';
CREATE DATABASE stud;

CREATE TABLE IF NOT EXISTS `User` (`id` INT NOT NULL AUTO_INCREMENT,`first_name` VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,`last_name` VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,`email` VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NULL,`title` VARCHAR(1024) CHARACTER SET utf8 COLLATE utf8_unicode_ci NULL,PRIMARY KEY (`id`),KEY `fn` (`first_name`),KEY `ln` (`last_name`));

CREATE TABLE IF NOT EXISTS `Order` (`id` INT NOT NULL AUTO_INCREMENT,`Name` VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,`Price` int,`description` VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NULL, PRIMARY KEY (`id`), KEY `fn` (`Name`),KEY `ln` (`Price`));

CREATE TABLE IF NOT EXISTS `Product` (`id` INT NOT NULL AUTO_INCREMENT,`Name` VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,`Price` int,`description` VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NULL, PRIMARY KEY (`id`), KEY `fn` (`Name`),KEY `ln` (`Price`));

INSERT INTO Author (first_name,last_name,email,title) VALUES ('Иван','Иванов','ivanov@yandex.ru','господин');

##  Запуск
sudo ./build/hl_mai_lab_01 --host=localhost --port=3306 --login=stud --password=stud --database=stud

# Запросы
index.yaml
