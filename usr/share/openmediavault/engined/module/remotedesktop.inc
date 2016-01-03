<?php

/**
 * Copyright (C) 2009-2013 Volker Theile <volker.theile@openmediavault.org>
 * Copyright (C) 2013-2016 OpenMediaVault Plugin Developers
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

require_once "openmediavault/config.inc";
require_once "openmediavault/error.inc";
require_once "openmediavault/module.inc";

class OMVModuleRemoteDesktop extends OMVModuleServiceAbstract implements
    OMVINotifyListener
{
    public function getName()
    {
        return "RemoteDesktop";
    }

    private function getPluginName()
    {
        return strtolower($this->getName());
    }

    private function getXPath()
    {
        return sprintf("/config/services/%s", $this->getPluginName());
    }

    private function getEventMessagePath()
    {
        return sprintf("org.openmediavault.services.%s", $this->getPluginName());
    }

    public function applyConfig()
    {
        global $xmlConfig;

        $cmd = sprintf("omv-mkconf %s 2>&1", $this->getPluginName());

        if (0 !== $this->exec($cmd, $output)) {
            throw new OMVException(
                OMVErrorMsg::E_EXEC_FAILED,
                $cmd,
                implode("\n", $output)
            );
        }
    }

    public function bindListeners(OMVNotifyDispatcher $dispatcher)
    {
        $dispatcher->addListener(
            OMV_NOTIFY_MODIFY,
            $this->getEventMessagePath(),
            [ $this, "setDirty" ]
        );
    }
}

// Register module.
$moduleMgr = &OMVModuleMgr::getInstance();
$moduleMgr->registerModule(new OMVModuleRemoteDesktop());
