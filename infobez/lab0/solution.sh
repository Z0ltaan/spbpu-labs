#!/usr/bin/env bash

TESTUSER="testuser1"
2_2() {
  useradd $TESTUSER

  echo -e "a\na" | passwd $TESTUSER

  passwd -S $TESTUSER

  passwd -x 5 $TESTUSER

  passwd -w 10 $TESTUSER

  passwd -S $TESTUSER

  echo -e "a\n" | su - $TESTUSER
}

TEAM_M="$TESTUSER sysadmin"

2_3() {
  groupadd team

  for member in $TEAM_M; do
    usermod -aG team $member
  done

  for member in $TEAM_M; do
    id $member
  done
}

2_4() {
  mkdir -p /srv/project
  chown root:team /srv/project
  chmod 770 /srv/project

  sudo -u $TESTUSER touch /srv/project/file_test1
  sudo -u $TESTUSER mkdir /srv/project/dir_test1

  ls -al /srv/project

  rm -rf /srv/project/file_test1 /srv/project/dir_test1
}

2_5() {
  mkdir -p /srv/project
  chown root:team /srv/project
  chmod 770 /srv/project

  chmod g+s /srv/project

  sudo -u $TESTUSER touch /srv/project/file_test2
  sudo -u $TESTUSER mkdir /srv/project/dir_test2

  ls -al /srv/project

  rm -rf /srv/project/file_test2 /srv/project/dir_test2
}

main() {
  if [[ -z "${EXECUTION_LIST}" ]]; then
    EXECUTION_LIST="2_2 2_3 2_4 2_5"
  fi

  for task in $EXECUTION_LIST; do
    $task
  done
}

main
