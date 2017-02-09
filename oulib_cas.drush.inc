<?php

/*
 * Implements hook_drush_command();
 */
function oulib_cas_drush_command() {

  $items['oulib-cassify'] = array(
    'description' => 'Set auth map for user',
    );
  return $items;

}

/*
 * Callback for the set-auth-map command
 */
function drush_oulib_cas_oulib_cassify() {

  $transaction = db_transaction();
  try {

    /* Set cas user name for any account that lacks it */
    $query_convert=db_query(
      '
INSERT INTO {cas_user} (uid, cas_name)
     SELECT u.uid AS uid, u.name as cas_name
       FROM {users} u
      WHERE uid <> 0
        AND NOT EXISTS (SELECT cas_name
                          FROM {cas_user} c
                         WHERE c.cas_name = u.name);
'
    )->execute();
  } catch (\PDOException $e) {
    $transaction->rollback();
    $error = $e->getMessage();
    drush_print($error);
  }
  unset($transaction); // commit

}