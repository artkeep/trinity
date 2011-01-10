/*Table structure for table `arena_statistics` */

DROP TABLE IF EXISTS `arena_statistics`;

CREATE TABLE `arena_statistics` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `type` int(11) NOT NULL DEFAULT '0',
  `winner_id` int(11) NOT NULL DEFAULT '0',
  `winner_rating` int(11) NOT NULL DEFAULT '0',
  `winner_change` int(11) NOT NULL DEFAULT '0',
  `loser_id` int(11) NOT NULL DEFAULT '0',
  `loser_rating` int(11) NOT NULL DEFAULT '0',
  `loser_change` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;