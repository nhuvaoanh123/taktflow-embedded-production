"""
CanTp_Cfg generator — produces CanTp_Cfg_*.c for each ECU with UDS diagnostics.

Generates ISO-TP channel configuration that maps:
  RX: CanIf RX PDU (UDS request) → CanTp → Dcm
  TX: Dcm → CanTp → CanIf TX PDU (UDS response)

Only generates if the ECU has uds_rx_pdu_name configured in the sidecar.
"""

from __future__ import annotations

import os

from ..engine import OutputFile, TemplateEngine
from ..model import Ecu


name = "cantp"


class CanTpCfgGenerator:
    """Generates CanTp_Cfg.c for each ECU."""

    def render(self, ecu: Ecu, gen_config, engine: TemplateEngine) -> dict[str, str]:
        context = {
            "ecu": ecu,
            "filename": f"CanTp_Cfg_{ecu.prefix.capitalize()}.c",
            "brief": f"CanTp configuration for {ecu.prefix} — ISO-TP for UDS diagnostics",
            "tool_version": "1.0.0",
            "arxml_source": "TaktflowSystem.arxml",
        }

        content = engine.render("cantp/CanTp_Cfg.c.j2", context)
        return {"CanTp_Cfg.c": content}

    def generate(self, ecu: Ecu, gen_config, engine: TemplateEngine) -> list[OutputFile]:
        # Skip if ECU has no UDS configured
        if not ecu.uds_rx_pdu_name:
            return []

        rendered = self.render(ecu, gen_config, engine)
        results = []

        for filename, content in rendered.items():
            output_name = f"CanTp_Cfg_{ecu.prefix.capitalize()}.c"
            path = os.path.join(
                engine.config.project_root,
                engine.config.output.base_dir,
                ecu.name,
                engine.config.output.cfg_dir,
                output_name,
            )
            result = engine.write_file(path, content)
            result.template = "cantp/CanTp_Cfg.c.j2"
            results.append(result)

        return results
