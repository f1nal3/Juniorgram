CREATE TABLE users ( id serial PRIMARY KEY, email varchar(32) UNIQUE NOT NULL, login varchar(128) UNIQUE NOT NULL, password_hash varchar(128) UNIQUE NOT NULL );

CREATE TABLE user_personal_data ( user_id int NOT NULL, first_name varchar(32), second_name varchar(32), birthday date, sex boolean );

CREATE TABLE user_tokens ( user_id int UNIQUE, token varchar(64) NOT NULL, refresh_token varchar(64) NOT NULL, token_receipt_time timestamp NOT NULL );

CREATE TABLE user_friends_data ( user_id int NOT NULL, friend_id int UNIQUE NOT NULL );

CREATE TABLE user_channles ( user_id int NOT NULL, channel_id int NOT NULL );

CREATE TABLE msgs ( msg_id serial PRIMARY KEY, sender_id int NOT NULL, send_time timestamp, msg text NOT NULL );

CREATE TABLE msg_reactions ( msg_id int NOT NULL, like_number int DEFAULT 0, dislike_number int DEFAULT 0, fire_number int DEFAULT 0, cat_number int DEFAULT 0 );

CREATE TABLE user_from_msgs ( from_id int NOT NULL, msg_id int NOT NULL );

CREATE TABLE user_to_msgs ( to_id int NOT NULL, msg_id int NOT NULL );

CREATE TABLE channel_msgs ( channel_id int NOT NULL, msg_id int NOT NULL );

CREATE TABLE channels ( id serial PRIMARY KEY, channel_name varchar(64) UNIQUE, creator_id int NOT NULL );

CREATE TABLE channel_images ( channel_id int NOT NULL, image_id int NOT NULL, save_date timestamp NOT NULL, sender_id int NOT NULL );

CREATE TABLE channel_files ( channel_id int NOT NULL, file_id int NOT NULL, save_date timestamp NOT NULL, sender_id int NOT NULL );

CREATE TABLE channel_videos ( channel_id int NOT NULL, video_id int NOT NULL, save_date timestamp NOT NULL, sender_id int NOT NULL );

CREATE TABLE channel_audio ( channel_id int NOT NULL, audio_id int NOT NULL, save_date timestamp NOT NULL, sender_id int NOT NULL );

CREATE TABLE channel_links ( channel_id int NOT NULL, link_id int NOT NULL, save_date timestamp NOT NULL, sender_id int NOT NULL );

CREATE TABLE images ( image_id serial PRIMARY KEY, image_name varchar(32) NOT NULL, image_data bytea NOT NULL );

CREATE TABLE files ( file_id serial PRIMARY KEY, file_name varchar(32) NOT NULL, file_data bytea NOT NULL );

CREATE TABLE videos ( video_id serial PRIMARY KEY, video_name varchar(32) NOT NULL, video_data bytea NOT NULL );

CREATE TABLE audio ( audio_id serial PRIMARY KEY, audio_name varchar(32) NOT NULL, audio_data bytea NOT NULL );

CREATE TABLE links ( link_id serial PRIMARY KEY, link varchar(64) NOT NULL );

ALTER TABLE user_personal_data ADD FOREIGN KEY (user_id) REFERENCES users (id) ON DELETE CASCADE;

ALTER TABLE user_tokens ADD FOREIGN KEY (user_id) REFERENCES users (id) ON DELETE SET NULL;

ALTER TABLE user_friends_data ADD FOREIGN KEY (user_id) REFERENCES users (id) ON DELETE SET NULL;

ALTER TABLE user_friends_data ADD FOREIGN KEY (friend_id) REFERENCES users (id) ON DELETE SET NULL;

ALTER TABLE user_channles ADD FOREIGN KEY (user_id) REFERENCES users (id) ON DELETE SET NULL;

ALTER TABLE user_channles ADD FOREIGN KEY (channel_id) REFERENCES channels (id) ON DELETE SET NULL;

ALTER TABLE msg_reactions ADD FOREIGN KEY (msg_id) REFERENCES msgs (msg_id) ON DELETE SET NULL;

ALTER TABLE user_from_msgs ADD FOREIGN KEY (from_id) REFERENCES users (id) ON DELETE SET NULL;

ALTER TABLE user_from_msgs ADD FOREIGN KEY (msg_id) REFERENCES msgs (msg_id) ON DELETE CASCADE;

ALTER TABLE user_to_msgs ADD FOREIGN KEY (to_id) REFERENCES users (id) ON DELETE SET NULL;

ALTER TABLE user_to_msgs ADD FOREIGN KEY (msg_id) REFERENCES msgs (msg_id) ON DELETE CASCADE;

ALTER TABLE channel_msgs ADD FOREIGN KEY (channel_id) REFERENCES channels (id) ON DELETE SET NULL;

ALTER TABLE channel_msgs ADD FOREIGN KEY (msg_id) REFERENCES msgs (msg_id) ON DELETE CASCADE;

ALTER TABLE channels ADD FOREIGN KEY (creator_id) REFERENCES users (id) ON DELETE CASCADE;

ALTER TABLE channel_images ADD FOREIGN KEY (channel_id) REFERENCES channels (id) ON DELETE SET NULL;

ALTER TABLE channel_images ADD FOREIGN KEY (image_id) REFERENCES images (image_id) ON DELETE CASCADE;

ALTER TABLE channel_images ADD FOREIGN KEY (sender_id) REFERENCES users (id) ON DELETE SET NULL;

ALTER TABLE channel_files ADD FOREIGN KEY (channel_id) REFERENCES channels (id) ON DELETE SET NULL;

ALTER TABLE channel_files ADD FOREIGN KEY (file_id) REFERENCES files (file_id) ON DELETE CASCADE;

ALTER TABLE channel_files ADD FOREIGN KEY (sender_id) REFERENCES users (id) ON DELETE SET NULL;

ALTER TABLE channel_videos ADD FOREIGN KEY (channel_id) REFERENCES channels (id) ON DELETE SET NULL;

ALTER TABLE channel_videos ADD FOREIGN KEY (video_id) REFERENCES videos (video_id) ON DELETE CASCADE;

ALTER TABLE channel_videos ADD FOREIGN KEY (sender_id) REFERENCES users (id) ON DELETE SET NULL;

ALTER TABLE channel_audio ADD FOREIGN KEY (channel_id) REFERENCES channels (id) ON DELETE SET NULL;

ALTER TABLE channel_audio ADD FOREIGN KEY (audio_id) REFERENCES audio (audio_id) ON DELETE CASCADE;

ALTER TABLE channel_audio ADD FOREIGN KEY (sender_id) REFERENCES users (id) ON DELETE SET NULL;

ALTER TABLE channel_links ADD FOREIGN KEY (channel_id) REFERENCES channels (id)ON DELETE SET NULL;

ALTER TABLE channel_links ADD FOREIGN KEY (link_id) REFERENCES links (link_id) ON DELETE CASCADE;

ALTER TABLE channel_links ADD FOREIGN KEY (sender_id) REFERENCES users (id) ON DELETE SET NULL;
 
