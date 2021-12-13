BEGIN TRANSACTION;
DROP TABLE IF EXISTS "short_desc";
CREATE TABLE IF NOT EXISTS "short_desc" (
	"id"	INTEGER NOT NULL,
	"adjective"	TEXT NOT NULL,
	"object_id"	INTEGER NOT NULL,
	PRIMARY KEY("id" AUTOINCREMENT)
);
DROP TABLE IF EXISTS "verbs";
CREATE TABLE IF NOT EXISTS "verbs" (
	"id"	INTEGER NOT NULL,
	"verb"	TEXT NOT NULL UNIQUE,
	"verb_id"	INTEGER NOT NULL DEFAULT 0,
	PRIMARY KEY("id" AUTOINCREMENT)
);
DROP TABLE IF EXISTS "object";
CREATE TABLE IF NOT EXISTS "object" (
	"id"	INTEGER NOT NULL,
	"name"	TEXT NOT NULL,
	"holder"	INTEGER,
	"short_desc"	TEXT,
	"first_time_desc"	TEXT,
	"desc"	TEXT,
	"N"	INTEGER,
	"E"	INTEGER,
	"W"	INTEGER,
	"S"	INTEGER,
	"U"	INTEGER,
	"D"	INTEGER,
	"is_viewed"	INTEGER NOT NULL DEFAULT 0,
	"is_getable"	INTEGER NOT NULL DEFAULT 0,
	PRIMARY KEY("id" AUTOINCREMENT)
);
INSERT INTO "verbs" ("id","verb","verb_id") VALUES (0,'get',0);
INSERT INTO "verbs" ("id","verb","verb_id") VALUES (1,'look',1);
INSERT INTO "verbs" ("id","verb","verb_id") VALUES (2,'examine',2);
INSERT INTO "verbs" ("id","verb","verb_id") VALUES (3,'drop',3);
INSERT INTO "verbs" ("id","verb","verb_id") VALUES (4,'n',4);
INSERT INTO "verbs" ("id","verb","verb_id") VALUES (5,'e',5);
INSERT INTO "verbs" ("id","verb","verb_id") VALUES (6,'w',6);
INSERT INTO "verbs" ("id","verb","verb_id") VALUES (7,'s',7);
INSERT INTO "verbs" ("id","verb","verb_id") VALUES (8,'u',8);
INSERT INTO "verbs" ("id","verb","verb_id") VALUES (9,'d',9);
INSERT INTO "verbs" ("id","verb","verb_id") VALUES (10,'north',4);
INSERT INTO "verbs" ("id","verb","verb_id") VALUES (11,'east',5);
INSERT INTO "verbs" ("id","verb","verb_id") VALUES (12,'west',6);
INSERT INTO "verbs" ("id","verb","verb_id") VALUES (13,'south',7);
INSERT INTO "verbs" ("id","verb","verb_id") VALUES (14,'up',8);
INSERT INTO "verbs" ("id","verb","verb_id") VALUES (15,'down',9);
INSERT INTO "verbs" ("id","verb","verb_id") VALUES (16,'take',0);
INSERT INTO "verbs" ("id","verb","verb_id") VALUES (17,'pick up',0);
INSERT INTO "verbs" ("id","verb","verb_id") VALUES (18,'look at',1);
INSERT INTO "verbs" ("id","verb","verb_id") VALUES (19,'inspect',2);
INSERT INTO "verbs" ("id","verb","verb_id") VALUES (20,'put down',3);
INSERT INTO "verbs" ("id","verb","verb_id") VALUES (21,'help',21);
INSERT INTO "verbs" ("id","verb","verb_id") VALUES (22,'exit',22);
INSERT INTO "object" ("id","name","holder","short_desc","first_time_desc","desc","N","E","W","S","U","D","is_viewed","is_getable") VALUES (0,'offscreen',NULL,NULL,NULL,'Leroy''s Quest

Author: Steven Savold

<maybe write an intro here?>
',NULL,NULL,NULL,NULL,NULL,NULL,0,0);
INSERT INTO "object" ("id","name","holder","short_desc","first_time_desc","desc","N","E","W","S","U","D","is_viewed","is_getable") VALUES (1,'player',2,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,0);
INSERT INTO "object" ("id","name","holder","short_desc","first_time_desc","desc","N","E","W","S","U","D","is_viewed","is_getable") VALUES (2,'Leroy''s Tower',NULL,NULL,NULL,'You stand before a tall tower. It looks old and weathered, but based on the rumors there is a large sum of treasure inside. It is locked behind the towers door. The key to which is said to have been lost in the Red Chasm. A place no mortal has ever returned from...',3,9,4,7,NULL,NULL,0,0);
INSERT INTO "object" ("id","name","holder","short_desc","first_time_desc","desc","N","E","W","S","U","D","is_viewed","is_getable") VALUES (3,'Treasureland',NULL,NULL,NULL,'Laid out before you is a vast expanse of trees as tall as giants. This forest, known as treasureland, is filled with acorns and all sorts of small creatures.',NULL,9,4,2,NULL,NULL,0,0);
INSERT INTO "object" ("id","name","holder","short_desc","first_time_desc","desc","N","E","W","S","U","D","is_viewed","is_getable") VALUES (4,'Heaven''s Peak',NULL,NULL,NULL,'The tall mountains tower above the surrounding landscapes. They make passage through the area difficult, but not impossible. The snow topped peaks and treacherous cliff sides leave little room for land dwelling creatures to live here.',5,3,7,2,NULL,NULL,0,0);
INSERT INTO "object" ("id","name","holder","short_desc","first_time_desc","desc","N","E","W","S","U","D","is_viewed","is_getable") VALUES (5,'Mermaid Cove',NULL,NULL,NULL,'In a small grove held within a patch of green lies a small lake of crystal blues and whites. The water is still except for the small rings of disturbance coming from behind a rock near the lakes edge.',NULL,NULL,NULL,4,NULL,NULL,0,0);
INSERT INTO "object" ("id","name","holder","short_desc","first_time_desc","desc","N","E","W","S","U","D","is_viewed","is_getable") VALUES (6,'Wish Well',NULL,NULL,NULL,'Alone a top a small hill sits a well made of wood and stone.',NULL,7,NULL,NULL,NULL,NULL,0,0);
INSERT INTO "object" ("id","name","holder","short_desc","first_time_desc","desc","N","E","W","S","U","D","is_viewed","is_getable") VALUES (7,'The Red Chasm',NULL,NULL,NULL,'You walk up to the chasms edge, the heat from the red flames pours into the air around you. If you walk any closer you might just burst into flames yourself.',4,2,6,9,NULL,NULL,0,0);
INSERT INTO "object" ("id","name","holder","short_desc","first_time_desc","desc","N","E","W","S","U","D","is_viewed","is_getable") VALUES (8,'Falcon Keep',NULL,NULL,NULL,'You walk out onto a beach of pale yellow sand. The tall cliffs above you seem almost as if they could crumble any second. Inset into these cliffs is a metal alter. Once used to give gifts to the those who ruled this forgotten place, Falcon Keep.',NULL,9,NULL,NULL,NULL,NULL,0,0);
INSERT INTO "object" ("id","name","holder","short_desc","first_time_desc","desc","N","E","W","S","U","D","is_viewed","is_getable") VALUES (9,'The Braids',NULL,NULL,NULL,'You approach an area with many hair like fibers stretching high into the sky. They are woven together making them very tough to pull apart and move past.',2,3,7,8,NULL,NULL,0,0);
INSERT INTO "object" ("id","name","holder","short_desc","first_time_desc","desc","N","E","W","S","U","D","is_viewed","is_getable") VALUES (10,'hair brush',3,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,1);
INSERT INTO "object" ("id","name","holder","short_desc","first_time_desc","desc","N","E","W","S","U","D","is_viewed","is_getable") VALUES (11,'song stone',3,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,1);
INSERT INTO "object" ("id","name","holder","short_desc","first_time_desc","desc","N","E","W","S","U","D","is_viewed","is_getable") VALUES (12,'feather',4,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,1);
INSERT INTO "object" ("id","name","holder","short_desc","first_time_desc","desc","N","E","W","S","U","D","is_viewed","is_getable") VALUES (13,'bucket',5,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,1);
INSERT INTO "object" ("id","name","holder","short_desc","first_time_desc","desc","N","E","W","S","U","D","is_viewed","is_getable") VALUES (14,'scissors',6,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,1);
INSERT INTO "object" ("id","name","holder","short_desc","first_time_desc","desc","N","E","W","S","U","D","is_viewed","is_getable") VALUES (15,'tower key',7,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,1);
INSERT INTO "object" ("id","name","holder","short_desc","first_time_desc","desc","N","E","W","S","U","D","is_viewed","is_getable") VALUES (16,'fireproof clothes',8,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,1);
COMMIT;
