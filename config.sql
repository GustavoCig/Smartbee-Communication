USE smartbee_projeto;

CREATE TABLE IF NOT EXISTS `smartbee_projeto`.`users` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(191) COLLATE utf8mb4_unicode_ci NOT NULL,
  `email` varchar(191) COLLATE utf8mb4_unicode_ci NOT NULL,
  `address` varchar(191) COLLATE utf8mb4_unicode_ci NOT NULL,
  `phone` varchar(191) COLLATE utf8mb4_unicode_ci NOT NULL,
  `cellphone` varchar(191) COLLATE utf8mb4_unicode_ci NOT NULL,
  `password` varchar(191) COLLATE utf8mb4_unicode_ci NOT NULL,
  `confirmed` tinyint(1) NOT NULL DEFAULT '0',
  `admin` tinyint(1) NOT NULL DEFAULT '0',
  `last_login` timestamp NULL DEFAULT NULL,
  `remember_token` varchar(255) NULL DEFAULT NULL,
  `login_alteracao` int(11) unsigned NULL,
  `created_at` timestamp NULL DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT NULL,
  `deleted_at` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `users_email_unique` (`email`),
  KEY `fk_users_login_alteracao` (`login_alteracao`),
  CONSTRAINT `fk_users_login_alteracao` FOREIGN KEY (`login_alteracao`) REFERENCES `users` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `smartbee_projeto`.`apiario` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `nome` varchar(255) NOT NULL,
  `endereco` varchar(255) NOT NULL,
  `latitude` varchar(255) DEFAULT NULL,
  `longitude` varchar(255) DEFAULT NULL,
  `descricao` text NOT NULL,
  `login_cadastro` int(11) unsigned NOT NULL,
  `login_alteracao` int(11) unsigned NOT NULL,
  `created_at` timestamp NULL DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT NULL,
  `deleted_at` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_apiario_login_cadastro` (`login_cadastro`),
  KEY `fk_apiario_login_alteracao` (`login_alteracao`),
  CONSTRAINT `fk_apiario_login_alteracao` FOREIGN KEY (`login_alteracao`) REFERENCES `users` (`id`),
  CONSTRAINT `fk_apiario_login_cadastro` FOREIGN KEY (`login_cadastro`) REFERENCES `users` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `smartbee_projeto`.`sensor` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `tipo` varchar(255) NOT NULL,
  `descricao` varchar(255) NOT NULL,
  `modelo` varchar(255) NOT NULL,
  `marca` varchar(255) NOT NULL,
  `login_cadastro` int(11) unsigned NOT NULL,
  `login_alteracao` int(11) unsigned NOT NULL,
  `created_at` timestamp NULL DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT NULL,
  `deleted_at` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_sensor_login_cadastro` (`login_cadastro`),
  KEY `fk_sensor_login_alteracao` (`login_alteracao`),
  CONSTRAINT `fk_sensor_login_alteracao` FOREIGN KEY (`login_alteracao`) REFERENCES `users` (`id`),
  CONSTRAINT `fk_sensor_login_cadastro` FOREIGN KEY (`login_cadastro`) REFERENCES `users` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `smartbee_projeto`.`colmeia_estado` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `nome` varchar(255) NOT NULL,
  `descricao` text NOT NULL,
  `login_cadastro` int(11) unsigned NOT NULL,
  `login_alteracao` int(11) unsigned NOT NULL,
  `created_at` timestamp NULL DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT NULL,
  `deleted_at` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_colmeia_estado_login_cadastro` (`login_cadastro`),
  KEY `fk_colmeia_estado_login_alteracao` (`login_alteracao`),
  CONSTRAINT `fk_colmeia_estado_login_alteracao` FOREIGN KEY (`login_alteracao`) REFERENCES `users` (`id`),
  CONSTRAINT `fk_colmeia_estado_login_cadastro` FOREIGN KEY (`login_cadastro`) REFERENCES `users` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `smartbee_projeto`.`colmeia_base` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `nome` varchar(255) NOT NULL,
  `descricao` text NOT NULL,
  `login_cadastro` int(11) unsigned NOT NULL,
  `login_alteracao` int(11) unsigned NOT NULL,
  `created_at` timestamp NULL DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT NULL,
  `deleted_at` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_colmeia_base_login_cadastro` (`login_cadastro`),
  KEY `fk_colmeia_base_login_alteracao` (`login_alteracao`),
  CONSTRAINT `fk_colmeia_base_login_alteracao` FOREIGN KEY (`login_alteracao`) REFERENCES `users` (`id`),
  CONSTRAINT `fk_colmeia_base_login_cadastro` FOREIGN KEY (`login_cadastro`) REFERENCES `users` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `smartbee_projeto`.`colmeia_origem` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `nome` varchar(255) NOT NULL,
  `descricao` text NOT NULL,
  `login_cadastro` int(11) unsigned NOT NULL,
  `login_alteracao` int(11) unsigned NOT NULL,
  `created_at` timestamp NULL DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT NULL,
  `deleted_at` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_colmeia_origem_login_cadastro` (`login_cadastro`),
  KEY `fk_colmeia_origem_login_alteracao` (`login_alteracao`),
  CONSTRAINT `fk_colmeia_origem_login_alteracao` FOREIGN KEY (`login_alteracao`) REFERENCES `users` (`id`),
  CONSTRAINT `fk_colmeia_origem_login_cadastro` FOREIGN KEY (`login_cadastro`) REFERENCES `users` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `smartbee_projeto`.`colmeia_tipo` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `nome` varchar(255) NOT NULL,
  `descricao` text NOT NULL,
  `login_cadastro` int(11) unsigned NOT NULL,
  `login_alteracao` int(11) unsigned NOT NULL,
  `created_at` timestamp NULL DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT NULL,
  `deleted_at` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_colmeia_tipo_login_cadastro` (`login_cadastro`),
  KEY `fk_colmeia_tipo_login_alteracao` (`login_alteracao`),
  CONSTRAINT `fk_colmeia_tipo_login_alteracao` FOREIGN KEY (`login_alteracao`) REFERENCES `users` (`id`),
  CONSTRAINT `fk_colmeia_tipo_login_cadastro` FOREIGN KEY (`login_cadastro`) REFERENCES `users` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `smartbee_projeto`.`password_resets` (
  `email` varchar(255) COLLATE utf8mb4_unicode_ci NOT NULL,
  `token` varchar(255) COLLATE utf8mb4_unicode_ci NOT NULL,
  `created_at` timestamp NULL DEFAULT NULL,
  KEY `password_resets_email_index` (`email`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE IF NOT EXISTS `smartbee_projeto`.`colmeia` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `nome` varchar(255) NOT NULL,
  `caixilho` int(11) NOT NULL,
  `latitude` varchar(255) DEFAULT NULL,
  `longitude` varchar(255) DEFAULT NULL,
  `apiario_id` int(11) unsigned NOT NULL,
  `colmeia_tipo_id` int(11) unsigned NOT NULL,
  `colmeia_base_id` int(11) unsigned NOT NULL,
  `colmeia_origem_id` int(11) unsigned NOT NULL,
  `colmeia_estado_id` int(11) unsigned NOT NULL,
  `login_cadastro` int(11) unsigned NOT NULL,
  `login_alteracao` int(11) unsigned NOT NULL,
  `created_at` timestamp NULL DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT NULL,
  `deleted_at` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_colmeia_apiario` (`apiario_id`),
  KEY `fk_colmeia_tipo` (`colmeia_tipo_id`),
  KEY `fk_colmeia_base` (`colmeia_base_id`),
  KEY `fk_colmeia_origem` (`colmeia_origem_id`),
  KEY `fk_colmeia_estado` (`colmeia_estado_id`),
  KEY `fk_colmeia_cadastro` (`login_cadastro`),
  KEY `fk_colmeia_alteracao` (`login_alteracao`),
  CONSTRAINT `fk_colmeia_alteracao` FOREIGN KEY (`login_alteracao`) REFERENCES `users` (`id`),
  CONSTRAINT `fk_colmeia_cadastro` FOREIGN KEY (`login_cadastro`) REFERENCES `users` (`id`),
  CONSTRAINT `fk_colmeia_base` FOREIGN KEY (`colmeia_base_id`) REFERENCES `colmeia_base` (`id`),
  CONSTRAINT `fk_colmeia_origem` FOREIGN KEY (`colmeia_origem_id`) REFERENCES `colmeia_origem` (`id`),
  CONSTRAINT `fk_colmeia_tipo` FOREIGN KEY (`colmeia_tipo_id`) REFERENCES `colmeia_tipo` (`id`),
  CONSTRAINT `fk_colmeia_estado` FOREIGN KEY (`colmeia_estado_id`) REFERENCES `colmeia_estado` (`id`),
  CONSTRAINT `fk_colmeia_apiario` FOREIGN KEY (`apiario_id`) REFERENCES `apiario` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `smartbee_projeto`.`colmeia_coleta` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `colmeia_id` int(11) unsigned NOT NULL,
  `sensor_id` int(11) unsigned NOT NULL,
  `valor` text NOT NULL,
  `created_at` timestamp NULL DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT NULL,
  `deleted_at` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_colmeia_coleta_colmeia_id` (`colmeia_id`),
  KEY `fk_colmeia_coleta_sensor_id` (`sensor_id`),
  CONSTRAINT `fk_colmeia_coleta_colmeia_id` FOREIGN KEY (`colmeia_id`) REFERENCES `colmeia` (`id`),
  CONSTRAINT `fk_colmeia_coleta_sensor_id` FOREIGN KEY (`sensor_id`) REFERENCES `sensor` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE IF NOT EXISTS `smartbee_projeto`.`colmeia_fundo` (
  `id` INT(11) UNSIGNED NOT NULL AUTO_INCREMENT,
  `nome` VARCHAR(255) NOT NULL,
  `descricao` TEXT NOT NULL,
  `login_cadastro` INT(11) UNSIGNED NOT NULL,
  `login_alteracao` INT(11) UNSIGNED NOT NULL,
  `created_at` TIMESTAMP NULL DEFAULT NULL,
  `updated_at` TIMESTAMP NULL DEFAULT NULL,
  `deleted_at` TIMESTAMP NULL DEFAULT NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_colmeia_fundo_login_cadastro` (`login_cadastro` ASC),
  INDEX `fk_colmeia_fundo_login_alteracao` (`login_alteracao` ASC))
ENGINE = InnoDB;
