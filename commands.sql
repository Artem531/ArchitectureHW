DROP TABLE IF EXISTS `Service_Order`;
DROP TABLE IF EXISTS `UserOrder`;
DROP TABLE IF EXISTS `Service`;
DROP TABLE IF EXISTS `User`;

CREATE TABLE IF NOT EXISTS `User` (
`id` INT NOT NULL AUTO_INCREMENT, 
`first_name` VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
`last_name` VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
`email` VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NULL,
`title` VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NULL,
`type` VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NULL,
PRIMARY KEY (`id`), KEY `fn` (`first_name`), KEY `ln` (`last_name`)
);

CREATE TABLE IF NOT EXISTS `Service` (
`id` INT NOT NULL AUTO_INCREMENT,
`service_special_id` VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
`name` VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
`price` DECIMAL(10, 2),
PRIMARY KEY (`id`), UNIQUE KEY `s` (`service_special_id`), KEY `n` (`name`)
);

CREATE TABLE IF NOT EXISTS `UserOrder` (
`id` INT NOT NULL AUTO_INCREMENT,
`user_id` INT NOT NULL,
PRIMARY KEY (`id`),
FOREIGN KEY (`user_id`) REFERENCES `User`(`id`) ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS `Service_Order` (
`id` INT NOT NULL AUTO_INCREMENT,
`service_id` INT NOT NULL,
`order_id` INT NOT NULL,
PRIMARY KEY (`id`),
FOREIGN KEY (`service_id`) REFERENCES `Service`(`id`) ON UPDATE CASCADE ON DELETE CASCADE,
FOREIGN KEY (`order_id`) REFERENCES `UserOrder`(`id`) ON UPDATE CASCADE ON DELETE CASCADE
);

-- https://generatedata.com/generator
INSERT INTO `User`(`first_name`, `last_name`, `email`, `title`, `type`) VALUES
("Jin","Robles","molestie.sed@protonmail.couk","Ante Ipsum Associates","admin"),
("Dante","Hopkins","euismod@outlook.com","Fames Ac Turpis Consulting","seller"), 
("Lila","Mccoy","lorem.donec@yahoo.org","In Scelerisque Foundation","admin"), 
("Scott","Sims","morbi.non@aol.ca","Risus Donec Incorporated","seller"), 
("Noelle","Booth","massa.lobortis@yahoo.ca","Mollis Limited","admin");


INSERT INTO `Service`(`service_special_id`, `name`, `price`) VALUES
(9358,"Paki",140),
(3854,"Cailin",490), 
(7856,"Zane",212), 
(5565,"Erasmus",876), 
(8334,"Meredith",349);

INSERT INTO `UserOrder` (`user_id`) VALUES
(1),
(2),
(3), 
(4), 
(5);

INSERT INTO `Service_Order`(`service_id`, `order_id`) VALUES
(1, 1),
(3, 1),
(2, 3),
(2, 1),
(4, 2),
(1, 3);
